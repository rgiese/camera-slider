#include "inc/stdinc.h"

//
// Mutations
//

MovementProgram::Movement::Movement(MovementType const type,
                                    uint16_t const delayTime,
                                    int32_t const desiredPosition,
                                    uint32_t const desiredSpeed,
                                    uint32_t const desiredAcceleration,
                                    uint32_t const desiredDeceleration)
    : Type(type)
{
    DelayTime = delayTime;

    DesiredPosition =
        clamp(desiredPosition, MotorController::c_MinimumPosition_Steps, MotorController::c_MaxSafePosition_Steps);

    DesiredSpeed =
        clamp(desiredSpeed, MotorController::c_MinimumSpeed_StepsPerSec, MotorController::c_MaxSafeSpeed_StepsPerSec);

    DesiredAcceleration = clamp(desiredAcceleration,
                                MotorController::c_MinimumAcceleration_StepsPerSecPerSec,
                                MotorController::c_MaxSafeAcceleration_StepsPerSecPerSec);

    DesiredDeceleration = clamp(desiredDeceleration,
                                MotorController::c_MinimumAcceleration_StepsPerSecPerSec,
                                MotorController::c_MaxSafeAcceleration_StepsPerSecPerSec);
}

void MovementProgram::Movement::applyDelta(Parameter const parameter, int32_t delta)
{
    switch (parameter)
    {
        case Parameter::DesiredPosition:
            DesiredPosition = clamp_delta(DesiredPosition,
                                          delta,
                                          MotorController::c_MinimumPosition_Steps,
                                          MotorController::c_MaxSafePosition_Steps);
            return;

        case Parameter::DesiredSpeed:
            DesiredSpeed = clamp_delta(DesiredSpeed,
                                       delta,
                                       MotorController::c_MinimumSpeed_StepsPerSec,
                                       MotorController::c_MaxSafeSpeed_StepsPerSec);
            return;

        case Parameter::DesiredAcceleration:
            DesiredAcceleration = clamp_delta(DesiredAcceleration,
                                              delta,
                                              MotorController::c_MinimumAcceleration_StepsPerSecPerSec,
                                              MotorController::c_MaxSafeAcceleration_StepsPerSecPerSec);
            return;

        case Parameter::DesiredDeceleration:
            DesiredDeceleration = clamp_delta(DesiredDeceleration,
                                              delta,
                                              MotorController::c_MinimumAcceleration_StepsPerSecPerSec,
                                              MotorController::c_MaxSafeAcceleration_StepsPerSecPerSec);
            return;

        default:
            // Ignore
            return;
    }
}

void MovementProgram::applyRateDelta(int16_t const RateDelta)
{
    RatePercent = clamp_delta(RatePercent, RateDelta, RatePercent_Minimum, RatePercent_Maximum);
}

//
// Execution
//

void MovementProgram::requestMoveToMovement(size_t const idxMovement) const
{
    if (idxMovement >= Movements.size())
    {
        return;
    }

    Movement const& movement = Movements[idxMovement];

    // Configure acceleration/deceleration
    {
        Request request = {Type : RequestType::DesiredMaximumAcceleration};
        request.DesiredMaximumAcceleration.value =
            static_cast<uint32_t>(movement.DesiredAcceleration * RatePercent / 100.0f);
        g_RequestQueue.push(request);
    }

    {
        Request request = {Type : RequestType::DesiredMaximumDeceleration};
        request.DesiredMaximumDeceleration.value =
            static_cast<uint32_t>(movement.DesiredDeceleration * RatePercent / 100.0f);
        g_RequestQueue.push(request);
    }

    // Configure speed
    {
        Request request = {Type : RequestType::DesiredMaximumSpeed};
        request.DesiredMaximumSpeed.value = static_cast<uint32_t>(movement.DesiredSpeed * RatePercent / 100.0f);
        g_RequestQueue.push(request);
    }

    // Now go to position
    {
        Request request = {Type : RequestType::DesiredPosition};
        request.DesiredPosition.value = movement.DesiredPosition;
        g_RequestQueue.push(request);
    }
}


//
// Debugging
//

void MovementProgram::dump(char const* const szPrefix) const
{
    Serial.printlnf("Movement program (%s):", szPrefix);

    {
        if (Repeat)
        {
            Serial.println("  Flags");

            if (Repeat)
            {
                Serial.println("  - Repeat");
            }
        }

        Serial.printlnf("  Rate: %u", RatePercent);
    }

    if (!Movements.empty())
    {
        Serial.println("  Steps:");

        for (auto const& movement : Movements)
        {
            switch (movement.Type)
            {
                case MovementType::Move:
                    Serial.printlnf("    Move: to %ld steps, speed %lu steps/sec, accel %lu decel %lu steps/sec^2",
                                    movement.DesiredPosition,
                                    movement.DesiredSpeed,
                                    movement.DesiredAcceleration,
                                    movement.DesiredDeceleration);
                    break;

                case MovementType::Delay:
                    Serial.printlnf("    Delay: %lu msec", movement.DelayTime);
                    break;

                default:
                    Serial.printlnf("    UnknownMovementType(%u)", movement.Type);
                    break;
            }
        }
    }
}
