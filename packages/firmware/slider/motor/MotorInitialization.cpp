#include "inc/stdinc.h"

//
// Startup
//

bool MotorController::tryInitialize()
{
    // De-energize to make sure communications with motor controller are working correctly
    m_Tic.exitSafeStart();
    m_Tic.deenergize();

    if (m_Tic.getOperationState() != TicOperationState::Deenergized)
    {
        // Retry on next tick
        return false;
    }

    // Validate burned-in motor controller settings
    {
        // Command timeout (watchdog timer)
        uint16_t commandTimeout = 0;

        m_Tic.getSetting(0x09, sizeof(commandTimeout), reinterpret_cast<uint8_t*>(&commandTimeout));

        if (commandTimeout > 1000 /* msec */)
        {
            // Unsafe command timeout
            g_StateKeeper.RequestState(new UnrecoverableErrorState("Unsafe command timeout %ums", commandTimeout));
            return false;
        }
    }

    {
        // Soft error response should be to de-energize
        uint8_t softErrorResponse;

        m_Tic.getSetting(0x53, sizeof(softErrorResponse), &softErrorResponse);

        if (softErrorResponse != 0 /* de-energize */)
        {
            // Unsafe soft error response
            g_StateKeeper.RequestState(
                new UnrecoverableErrorState("Unsafe soft error response 0x%02x", softErrorResponse));
            return false;
        }
    }

    {
        // Should not auto-home
        uint8_t configBitSet02;

        enum class ConfigBitSet02
        {
            AutoHoming = 1,
        };

        m_Tic.getSetting(0x02, sizeof(configBitSet02), &configBitSet02);

        if (configBitSet02 & (1 << static_cast<int>(ConfigBitSet02::AutoHoming)))
        {
            g_StateKeeper.RequestState(new UnrecoverableErrorState("Auto-homing should be disabled"));
            return false;
        }
    }

    {
        // Should support safe start, should not auto-home forward
        uint8_t configBitSet03;

        enum class ConfigBitSet03
        {
            DisableSafeStart = 0,
            AutoHomingForward = 2,
        };

        m_Tic.getSetting(0x03, sizeof(configBitSet03), &configBitSet03);

        if (configBitSet03 & (1 << static_cast<int>(ConfigBitSet03::DisableSafeStart)))
        {
            g_StateKeeper.RequestState(new UnrecoverableErrorState("Safe start should be enabled"));
            return false;
        }

        if (configBitSet03 & (1 << static_cast<int>(ConfigBitSet03::AutoHomingForward)))
        {
            g_StateKeeper.RequestState(new UnrecoverableErrorState("Auto-homing forward should be disabled"));
            return false;
        }
    }

    {
        // Homing speeds should be safe
        uint32_t homingSpeedTowards;
        uint32_t homingSpeedAway;

        m_Tic.getSetting(0x61, sizeof(homingSpeedTowards), reinterpret_cast<uint8_t*>(&homingSpeedTowards));
        m_Tic.getSetting(0x65, sizeof(homingSpeedAway), reinterpret_cast<uint8_t*>(&homingSpeedAway));

        if (speedFromTicUnits(homingSpeedTowards) > c_MaxSafeHomingSpeed_StepsPerSec)
        {
            g_StateKeeper.RequestState(
                new UnrecoverableErrorState("Unsafe homing (towards) speed %u", speedFromTicUnits(homingSpeedTowards)));
            return false;
        }

        if (speedFromTicUnits(homingSpeedAway) > c_MaxSafeHomingSpeed_StepsPerSec)
        {
            g_StateKeeper.RequestState(
                new UnrecoverableErrorState("Unsafe homing (away) speed %u", speedFromTicUnits(homingSpeedAway)));
            return false;
        }
    }

    {
        // Limit switches should be set up correctly
        {
            // Limit switch forward
            uint8_t txPinConfiguration;

            m_Tic.getSetting(0x3D, sizeof(txPinConfiguration), &txPinConfiguration);

            if (txPinConfiguration != 8 /* limit switch forward */)
            {
                g_StateKeeper.RequestState(new UnrecoverableErrorState("Forward limit switch not configured"));
                return false;
            }

            uint8_t limitSwitchForwardMap;

            m_Tic.getSetting(0x5F, sizeof(limitSwitchForwardMap), &limitSwitchForwardMap);

            if (limitSwitchForwardMap != (1 << 2) /* limit switch forward is TX */)
            {
                g_StateKeeper.RequestState(new UnrecoverableErrorState("Limit switch forward misconfigured"));
                return false;
            }
        }

        {
            // Limit switch reverse
            uint8_t rxPinConfiguration;

            m_Tic.getSetting(0x3E, sizeof(rxPinConfiguration), &rxPinConfiguration);

            if (rxPinConfiguration != 9 /* limit switch reverse */)
            {
                g_StateKeeper.RequestState(new UnrecoverableErrorState("Reverse limit switch not configured"));
                return false;
            }

            uint8_t limitSwitchReverseMap;

            m_Tic.getSetting(0x60, sizeof(limitSwitchReverseMap), &limitSwitchReverseMap);

            if (limitSwitchReverseMap != (1 << 3) /* limit switch reverse is RX */)
            {
                g_StateKeeper.RequestState(new UnrecoverableErrorState("Limit switch reverse misconfigured"));
                return false;
            }
        }

        // Both limit switches
        uint8_t switchPolarityMap;

        m_Tic.getSetting(0x36, sizeof(switchPolarityMap), &switchPolarityMap);

        if (!(!!(switchPolarityMap & (1 << 2)) && !!(switchPolarityMap & (1 << 3))))
        {
            g_StateKeeper.RequestState(new UnrecoverableErrorState("Limit switch polarity misconfigured"));
            return false;
        }
    }

    // Apply configurable settings
    {
        m_Tic.setCurrentLimit(990 /* mA */);
        m_Tic.setStepMode(TicStepMode::Microstep8 /* 1/8th steps */);
    }

    {
        uint32_t const maxSpeed = speedToTicUnits(c_DefaultSpeed_StepsPerSec);

        m_Tic.setMaxSpeed(maxSpeed);
    }

    {
        uint32_t const maxAcceleration = accelerationToTicUnits(c_DefaultAcceleration_StepsPerSecPerSec);

        m_Tic.setMaxAccel(maxAcceleration);
        m_Tic.setMaxDecel(maxAcceleration);
    }

    // Post settings to BLE
    {
        g_Bluetooth.capabilitiesService().setMaximumSpeed(c_MaxSafeSpeed_StepsPerSec);
        g_Bluetooth.capabilitiesService().setMaximumAcceleration(c_MaxSafeAcceleration_StepsPerSecPerSec);
        g_Bluetooth.capabilitiesService().setMaximumPosition(c_MaxSafePosition_Steps);
    }

    // Energize
    m_Tic.exitSafeStart();
    m_Tic.energize();

    if (m_Tic.getOperationState() != TicOperationState::Normal)
    {
        // Retry on next tick
        return false;
    }

    m_Tic.exitSafeStart();

    return true;
}
