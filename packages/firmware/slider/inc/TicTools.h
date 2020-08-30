#pragma once

namespace TicTools
{
    namespace Acceleration
    {
        inline uint32_t fromTicUnits(uint32_t const stepsPerSecPer100Sec)
        {
            // Return steps per second per second
            return stepsPerSecPer100Sec / 100;
        }

        inline uint32_t toTicUnits(uint32_t const stepsPerSecPerSec)
        {
            // Return steps per second per 100 seconds
            return stepsPerSecPerSec * 100;
        }
    }  // namespace Acceleration

    namespace Speed
    {
        inline uint32_t fromTicUnits(uint32_t const stepsPer10KSec)
        {
            // Return steps per second
            return stepsPer10KSec / 10000;
        }

        inline uint32_t toTicUnits(uint32_t const stepsPerSec)
        {
            // Return steps per second per 10K seconds
            return stepsPerSec * 10000;
        }

        constexpr uint32_t c_MaxSafeHomingSpeed_StepsPerSec = 2000;

    }  // namespace Speed

    namespace Debug
    {
        inline void dumpMotorControllerState(TicBase& motorController)
        {
            Serial.println("Motor controller state:");

            // Controller state
            {
                TicOperationState const operationState = motorController.getOperationState();
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
                uint16_t const errorStatus = motorController.getErrorStatus();
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
                TicStepMode const stepMode = motorController.getStepMode();
                uint16_t const currentLimit_mA = motorController.getCurrentLimit();

                Serial.printlnf("  Step mode: %d", static_cast<uint8_t>(stepMode));
                Serial.printlnf("  Current limit: %u", currentLimit_mA);
            }

            // Motor configuration: logical
            {
                int32_t const maxSpeed = motorController.getMaxSpeed();
                int32_t const maxAcceleration = motorController.getMaxAccel();
                int32_t const maxDeceleration = motorController.getMaxDecel();

                Serial.printlnf("  Max speed: %d steps/sec", TicTools::Speed::fromTicUnits(maxSpeed));
                Serial.printlnf("  Max acceleration: %d steps/sec^2, max deceleration: %d steps/sec^2",
                                TicTools::Acceleration::fromTicUnits(maxAcceleration),
                                TicTools::Acceleration::fromTicUnits(maxDeceleration));

                TicPlanningMode const planningMode = motorController.getPlanningMode();

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
                int16_t const inputVoltage_mV = motorController.getVinVoltage();

                Serial.printlnf("  Input voltage: %d mV", inputVoltage_mV);
            }

            // Motor state: planning
            {
                int32_t const targetPosition = motorController.getTargetPosition();
                int32_t const targetVelocity = motorController.getTargetVelocity();

                Serial.printlnf("  Target position: %d, target velocity: %d", targetPosition, targetVelocity);
            }

            // Motor state: execution
            {
                bool const isPositionUncertain = motorController.getPositionUncertain();
                int32_t const currentPosition = motorController.getCurrentPosition();
                int32_t const currentVelocity = motorController.getCurrentVelocity();

                Serial.printlnf("  Current position (%s): %d, current velocity: %d",
                                isPositionUncertain ? "uncertain" : "certain",
                                currentPosition,
                                currentVelocity);

                bool const isForwardLimitActive = motorController.getForwardLimitActive();
                bool const isReverseLimitActive = motorController.getReverseLimitActive();
                bool const isHoming = motorController.getHomingActive();

                Serial.printlnf("  Forward limit: %s, reverse limit: %s, homing: %s",
                                isForwardLimitActive ? "yes" : "no",
                                isReverseLimitActive ? "yes" : "no",
                                isHoming ? "yes" : "no");
            }

            Serial.println("");
        }

    }  // namespace Debug
}  // namespace TicTools