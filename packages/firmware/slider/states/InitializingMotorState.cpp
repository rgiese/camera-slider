#include "../inc/stdinc.h"
#include "states.h"

void InitializingMotorState::onEnteringState()
{
    Display::set("Initializing motor");
}

void InitializingMotorState::onLoop()
{
    // De-energize to make sure communications with motor controller are working correctly
    g_MotorController.exitSafeStart();
    g_MotorController.deenergize();

    if (g_MotorController.getOperationState() != TicOperationState::Deenergized)
    {
        // Retry on next tick
        return;
    }

    // Validate burned-in motor controller settings
    {
        // Command timeout (watchdog timer)
        uint16_t commandTimeout = 0;

        g_MotorController.getSetting(0x09, sizeof(commandTimeout), reinterpret_cast<uint8_t*>(&commandTimeout));

        if (commandTimeout > 1000 /* msec */)
        {
            // Unsafe command timeout
            return g_StateKeeper.RequestState(
                new UnrecoverableErrorState("Unsafe command timeout %ums", commandTimeout));
        }
    }

    {
        // Soft error response should be to de-energize
        uint8_t softErrorResponse;

        g_MotorController.getSetting(0x53, sizeof(softErrorResponse), &softErrorResponse);

        if (softErrorResponse != 0 /* de-energize */)
        {
            // Unsafe soft error response
            return g_StateKeeper.RequestState(
                new UnrecoverableErrorState("Unsafe soft error response 0x%02x", softErrorResponse));
        }
    }

    {
        // Should not auto-home
        uint8_t configBitSet02;

        enum class ConfigBitSet02
        {
            AutoHoming = 1,
        };

        g_MotorController.getSetting(0x02, sizeof(configBitSet02), &configBitSet02);

        if (configBitSet02 & (1 << static_cast<int>(ConfigBitSet02::AutoHoming)))
        {
            return g_StateKeeper.RequestState(new UnrecoverableErrorState("Auto-homing should be disabled"));
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

        g_MotorController.getSetting(0x03, sizeof(configBitSet03), &configBitSet03);

        if (configBitSet03 & (1 << static_cast<int>(ConfigBitSet03::DisableSafeStart)))
        {
            return g_StateKeeper.RequestState(new UnrecoverableErrorState("Safe start should be enabled"));
        }

        if (configBitSet03 & (1 << static_cast<int>(ConfigBitSet03::AutoHomingForward)))
        {
            return g_StateKeeper.RequestState(new UnrecoverableErrorState("Auto-homing forward should be disabled"));
        }
    }

    {
        // Homing speeds should be safe
        uint32_t homingSpeedTowards;
        uint32_t homingSpeedAway;

        g_MotorController.getSetting(0x61, sizeof(homingSpeedTowards), reinterpret_cast<uint8_t*>(&homingSpeedTowards));
        g_MotorController.getSetting(0x65, sizeof(homingSpeedAway), reinterpret_cast<uint8_t*>(&homingSpeedAway));

        if (TicTools::Speed::fromTicUnits(homingSpeedTowards) > TicTools::Speed::c_MaxSafeHomingSpeed_StepsPerSec)
        {
            return g_StateKeeper.RequestState(new UnrecoverableErrorState(
                "Unsafe homing (towards) speed %u", TicTools::Speed::fromTicUnits(homingSpeedTowards)));
        }

        if (TicTools::Speed::fromTicUnits(homingSpeedAway) > TicTools::Speed::c_MaxSafeHomingSpeed_StepsPerSec)
        {
            return g_StateKeeper.RequestState(new UnrecoverableErrorState(
                "Unsafe homing (away) speed %u", TicTools::Speed::fromTicUnits(homingSpeedAway)));
        }
    }

    {
        // Limit switches should be set up correctly
        {
            // Limit switch forward
            uint8_t txPinConfiguration;

            g_MotorController.getSetting(0x3D, sizeof(txPinConfiguration), &txPinConfiguration);

            if (txPinConfiguration != 8 /* limit switch forward */)
            {
                return g_StateKeeper.RequestState(new UnrecoverableErrorState("Forward limit switch not configured"));
            }

            uint8_t limitSwitchForwardMap;

            g_MotorController.getSetting(0x5F, sizeof(limitSwitchForwardMap), &limitSwitchForwardMap);

            if (limitSwitchForwardMap != (1 << 2) /* limit switch forward is TX */)
            {
                return g_StateKeeper.RequestState(new UnrecoverableErrorState("Limit switch forward misconfigured"));
            }
        }

        {
            // Limit switch reverse
            uint8_t rxPinConfiguration;

            g_MotorController.getSetting(0x3E, sizeof(rxPinConfiguration), &rxPinConfiguration);

            if (rxPinConfiguration != 9 /* limit switch reverse */)
            {
                return g_StateKeeper.RequestState(new UnrecoverableErrorState("Reverse limit switch not configured"));
            }

            uint8_t limitSwitchReverseMap;

            g_MotorController.getSetting(0x60, sizeof(limitSwitchReverseMap), &limitSwitchReverseMap);

            if (limitSwitchReverseMap != (1 << 3) /* limit switch reverse is RX */)
            {
                return g_StateKeeper.RequestState(new UnrecoverableErrorState("Limit switch reverse misconfigured"));
            }
        }

        // Both limit switches
        uint8_t switchPolarityMap;

        g_MotorController.getSetting(0x36, sizeof(switchPolarityMap), &switchPolarityMap);

        if (!(!!(switchPolarityMap & (1 << 2)) && !!(switchPolarityMap & (1 << 3))))
        {
            return g_StateKeeper.RequestState(new UnrecoverableErrorState("Limit switch polarity misconfigured"));
        }
    }

    // Apply configurable settings
    {
        g_MotorController.setCurrentLimit(990 /* mA */);
        g_MotorController.setStepMode(TicStepMode::Microstep8 /* 1/8th steps */);
    }

    {
        uint32_t const maxAcceleration = TicTools::Acceleration::toTicUnits(10000 /* steps per second per second */);

        g_MotorController.setMaxAccel(maxAcceleration);
        g_MotorController.setMaxDecel(maxAcceleration);
    }

    // Energize
    g_MotorController.exitSafeStart();
    g_MotorController.energize();

    if (g_MotorController.getOperationState() != TicOperationState::Normal)
    {
        // Retry on next tick
        return;
    }

    g_MotorController.exitSafeStart();

    // Advance state
    return g_StateKeeper.RequestState(new MotorHomingReverseState());
}
