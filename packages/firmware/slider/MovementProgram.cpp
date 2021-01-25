#include "inc/stdinc.h"

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
    movementProgram.Rate = pSourceProgram->rate() / 100.0f;

    auto const pSourceMovements = pSourceProgram->movements();

    if (pSourceMovements)
    {
        for (auto const& sourceMovement : *pSourceMovements)
        {
            MovementProgram::Movement movement = {
                Type : sourceMovement->type(),
                DelayTime : sourceMovement->delayTime(),
                DesiredPosition : sourceMovement->desiredPosition(),
                DesiredSpeed : sourceMovement->desiredSpeed(),
                DesiredAcceleration : sourceMovement->desiredAcceleration()
            };

            movementProgram.Movements.emplace_back(movement);
        }
    }

    return true;
}

void MovementProgram::dump() const
{
    Serial.println("Movement program:");

    {
        if (static_cast<uint32_t>(Flags) != 0)
        {
            Serial.println("  Flags");

            if (!!(Flags & Flatbuffers::Firmware::MovementProgramFlags::Repeat))
            {
                Serial.println("  - Repeat");
            }
        }

        Serial.printlnf("  Rate: %.2f", Rate);
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
