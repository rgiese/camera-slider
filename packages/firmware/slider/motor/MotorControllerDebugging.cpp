#include "inc/stdinc.h"

void MotorController::dumpState()
{
    Serial.println("Motor controller state:");

    // Controller state
    {
        TicOperationState const operationState = m_Tic.getOperationState();
        Serial.print("  Operation state: ");

        switch (operationState)
        {
            case TicOperationState::Reset:
                Serial.println("reset");
                break;
            case TicOperationState::Deenergized:
                Serial.println("de-energized");
                break;
            case TicOperationState::SoftError:
                Serial.println("soft error");
                break;
            case TicOperationState::WaitingForErrLine:
                Serial.println("waiting for error line");
                break;
            case TicOperationState::StartingUp:
                Serial.println("starting up");
                break;
            case TicOperationState::Normal:
                Serial.println("normal");
                break;
            default:
                Serial.println("other");
                break;
        }
    }

    // Controller errors
    {
        uint16_t const errorStatus = m_Tic.getErrorStatus();
        Serial.print("  Errors: ");

        if (errorStatus & (1 << static_cast<uint8_t>(TicError::IntentionallyDeenergized)))
        {
            Serial.print("intentionally-deenergized ");
        }

        if (errorStatus & (1 << static_cast<uint8_t>(TicError::MotorDriverError)))
        {
            Serial.print("motor-driver-error ");
        }

        if (errorStatus & (1 << static_cast<uint8_t>(TicError::LowVin)))
        {
            Serial.print("low-vin ");
        }
        if (errorStatus & (1 << static_cast<uint8_t>(TicError::KillSwitch)))
        {
            Serial.print("kill-switch ");
        }

        if (errorStatus & (1 << static_cast<uint8_t>(TicError::RequiredInputInvalid)))
        {
            Serial.print("required-input-invalid ");
        }

        if (errorStatus & (1 << static_cast<uint8_t>(TicError::SerialError)))
        {
            Serial.print("serial-error ");
        }

        if (errorStatus & (1 << static_cast<uint8_t>(TicError::CommandTimeout)))
        {
            Serial.print("command-timeout ");
        }

        if (errorStatus & (1 << static_cast<uint8_t>(TicError::SafeStartViolation)))
        {
            Serial.print("safe-start-violation ");
        }

        if (errorStatus & (1 << static_cast<uint8_t>(TicError::ErrLineHigh)))
        {
            Serial.print("error-line-high ");
        }

        Serial.println("");
    }

    // Motor configuration: physical
    {
        TicStepMode const stepMode = m_Tic.getStepMode();
        uint16_t const currentLimit_mA = m_Tic.getCurrentLimit();

        Serial.printlnf("  Step mode: %d", static_cast<uint8_t>(stepMode));
        Serial.printlnf("  Current limit: %u", currentLimit_mA);
    }

    // Motor configuration: logical
    {
        int32_t const maxSpeed = m_Tic.getMaxSpeed();
        int32_t const maxAcceleration = m_Tic.getMaxAccel();
        int32_t const maxDeceleration = m_Tic.getMaxDecel();

        Serial.printlnf("  Max speed: %lu steps/sec", speedFromTicUnits(maxSpeed));
        Serial.printlnf("  Max acceleration: %lu steps/sec^2, max deceleration: %lu steps/sec^2",
                        accelerationFromTicUnits(maxAcceleration),
                        accelerationFromTicUnits(maxDeceleration));

        TicPlanningMode const planningMode = m_Tic.getPlanningMode();

        Serial.print("  Planning mode: ");

        switch (planningMode)
        {
            case TicPlanningMode::Off:
                Serial.println("off");
                break;

            case TicPlanningMode::TargetPosition:
                Serial.println("target position");
                break;

            case TicPlanningMode::TargetVelocity:
                Serial.println("target velocity");
                break;

            default:
                Serial.println("unknown");
                break;
        }
    }

    // Controller state: voltage
    {
        int16_t const inputVoltage_mV = m_Tic.getVinVoltage();

        Serial.printlnf("  Input voltage: %d mV", inputVoltage_mV);
    }

    // Motor state: planning
    {
        int32_t const targetPosition = m_Tic.getTargetPosition();
        int32_t const targetVelocity = m_Tic.getTargetVelocity();

        Serial.printlnf(
            "  Target position: %lu, target velocity: %lu", targetPosition, speedFromTicUnits(targetVelocity));
    }

    // Motor state: execution
    {
        bool const isPositionUncertain = m_Tic.getPositionUncertain();
        int32_t const currentPosition = m_Tic.getCurrentPosition();
        int32_t const currentVelocity = m_Tic.getCurrentVelocity();

        Serial.printlnf("  Current position (%s): %ld, current velocity: %ld",
                        isPositionUncertain ? "uncertain" : "certain",
                        currentPosition,
                        speedFromTicUnits(currentVelocity));

        bool const isForwardLimitActive = m_Tic.getForwardLimitActive();
        bool const isReverseLimitActive = m_Tic.getReverseLimitActive();
        bool const isHoming = m_Tic.getHomingActive();

        Serial.printlnf("  Forward limit: %s, reverse limit: %s, homing: %s",
                        isForwardLimitActive ? "yes" : "no",
                        isReverseLimitActive ? "yes" : "no",
                        isHoming ? "yes" : "no");
    }

    Serial.println("");
}