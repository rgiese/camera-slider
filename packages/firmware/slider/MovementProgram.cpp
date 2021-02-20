#include "inc/stdinc.h"

//
// Mutations
//

MovementProgram::Movement::Movement(Flatbuffers::Firmware::MovementType const type,
                                    uint16_t const delayTime,
                                    int32_t const desiredPosition,
                                    uint32_t const desiredSpeed,
                                    uint32_t const desiredAcceleration)
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
}

void MovementProgram::Movement::applyDeltas(int32_t const desiredPositionDelta,
                                            int32_t const desiredSpeedDelta,
                                            int32_t const desiredAccelerationDelta)
{
    DesiredPosition = clamp_delta(DesiredPosition,
                                  desiredPositionDelta,
                                  MotorController::c_MinimumPosition_Steps,
                                  MotorController::c_MaxSafePosition_Steps);
    DesiredSpeed = clamp_delta(DesiredSpeed,
                               desiredSpeedDelta,
                               MotorController::c_MinimumSpeed_StepsPerSec,
                               MotorController::c_MaxSafeSpeed_StepsPerSec);
    DesiredAcceleration = clamp_delta(DesiredAcceleration,
                                      desiredAccelerationDelta,
                                      MotorController::c_MinimumAcceleration_StepsPerSecPerSec,
                                      MotorController::c_MaxSafeAcceleration_StepsPerSecPerSec);
}

void MovementProgram::applyRateDelta(int16_t const RateDelta)
{
    RatePercent = clamp_delta(RatePercent, RateDelta, RatePercent_Minimum, RatePercent_Maximum);
}

//
// Conversions
//

bool MovementProgram::fromFlatbufferData(uint8_t const* const pData,
                                         size_t const cbData,
                                         _Out_ MovementProgram& movementProgram)
{
    // Validate
    if (!pData)
    {
        return false;
    }

    {
        flatbuffers::Verifier verifier(pData, cbData);

        if (!Flatbuffers::Firmware::VerifyMovementProgramBuffer(verifier))
        {
            return false;
        }
    }

    // Build
    movementProgram = MovementProgram();

    auto const pSourceProgram = Flatbuffers::Firmware::GetMovementProgram(pData);

    movementProgram.Flags = pSourceProgram->flags();
    movementProgram.RatePercent = clamp(pSourceProgram->rate(), RatePercent_Minimum, RatePercent_Maximum);

    auto const pSourceMovements = pSourceProgram->movements();

    if (pSourceMovements)
    {
        for (auto const& sourceMovement : *pSourceMovements)
        {
            MovementProgram::Movement movement(sourceMovement->type(),
                                               sourceMovement->delayTime(),
                                               sourceMovement->desiredPosition(),
                                               sourceMovement->desiredSpeed(),
                                               sourceMovement->desiredAcceleration());

            movementProgram.Movements.emplace_back(movement);
        }
    }

    return true;
}

void MovementProgram::toFlatbufferData(flatbuffers::FlatBufferBuilder& flatbufferBuilder) const
{
    std::vector<Flatbuffers::Firmware::Movement> movements;
    {
        for (auto const& movement : Movements)
        {
            movements.emplace_back(movement.Type,
                                   0 /* padding */,
                                   movement.DelayTime,
                                   movement.DesiredPosition,
                                   movement.DesiredSpeed,
                                   movement.DesiredAcceleration);
        }
    }

    auto const movementProgramRoot =
        Flatbuffers::Firmware::CreateMovementProgramDirect(flatbufferBuilder, Flags, RatePercent, &movements);

    Flatbuffers::Firmware::FinishMovementProgramBuffer(flatbufferBuilder, movementProgramRoot);
}

//
// Debugging
//

void MovementProgram::dump(char const* const szPrefix) const
{
    Serial.printlnf("Movement program (%s):", szPrefix);

    {
        if (static_cast<uint32_t>(Flags) != 0)
        {
            Serial.println("  Flags");

            if (!!(Flags & Flatbuffers::Firmware::MovementProgramFlags::Repeat))
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
                case Flatbuffers::Firmware::MovementType::Move:
                    Serial.printlnf("    Move: to %d steps, speed %u steps/sec, acceleration %u steps/sec^2",
                                    movement.DesiredPosition,
                                    movement.DesiredSpeed,
                                    movement.DesiredAcceleration);
                    break;

                case Flatbuffers::Firmware::MovementType::Delay:
                    Serial.printlnf("    Delay: %lu msec", movement.DelayTime);
                    break;

                default:
                    Serial.printlnf("    UnknownMovementType(%u)", movement.Type);
                    break;
            }
        }
    }
}
