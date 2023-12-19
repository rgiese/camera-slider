#include "inc/stdinc.h"

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

        Serial.printlnf("  Rate: %.2f", Rate);
    }

    if (!Movements.empty())
    {
        Serial.println("  Steps:");

        for (auto const& movement : Movements)
        {
            switch (movement.Type)
            {
                case MovementType::Move:
                    Serial.printlnf("    Move: to %d steps, speed %u steps/sec, acceleration %u steps/sec^2",
                                    movement.DesiredPosition,
                                    movement.DesiredSpeed,
                                    movement.DesiredAcceleration);
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
