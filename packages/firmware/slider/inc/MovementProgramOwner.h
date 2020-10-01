#pragma once

class MovementProgramOwner
{
public:
    static std::shared_ptr<MovementProgramOwner> Initialize(uint8_t const* const pData, size_t const cbData)
    {
        if (!pData)
        {
            return std::shared_ptr<MovementProgramOwner>();
        }

        {
            flatbuffers::Verifier verifier(pData, cbData);

            if (!Flatbuffers::Firmware::VerifyMovementProgramBuffer(verifier))
            {
                return std::shared_ptr<MovementProgramOwner>();
            }
        }

        return std::shared_ptr<MovementProgramOwner>(new MovementProgramOwner(pData, cbData));
    }

    ~MovementProgramOwner()
    {
        delete[] m_Flatbuffer;
    }

    Flatbuffers::Firmware::MovementProgram const& get() const
    {
        return *m_MovementProgram;
    }

private:
    MovementProgramOwner(uint8_t const* const pData, size_t const cbData)
        : m_Flatbuffer(new uint8_t[cbData])
        , m_MovementProgram()
    {
        memcpy(const_cast<uint8_t*>(m_Flatbuffer), pData, cbData);
        m_MovementProgram = Flatbuffers::Firmware::GetMovementProgram(m_Flatbuffer);
    }

private:
    uint8_t const* m_Flatbuffer;
    Flatbuffers::Firmware::MovementProgram const* m_MovementProgram;

private:
    // Non-copyable
    MovementProgramOwner(MovementProgramOwner const&) = delete;
    MovementProgramOwner& operator=(MovementProgramOwner const&) = delete;

public:
    // Debugging tools
    void dump() const
    {
        Serial.println("Movement program:");

        {
            auto const movementProgramFlags = m_MovementProgram->flags();

            if (static_cast<uint32_t>(movementProgramFlags) != 0)
            {
                Serial.println("  Flags");

                if (!!(movementProgramFlags & Flatbuffers::Firmware::MovementProgramFlags::Repeat))
                {
                    Serial.println("  - Repeat");
                }
            }

            Serial.printlnf("  Rate: %.2f", static_cast<double>(m_MovementProgram->rate_minus1_x100() - 1) / 100);
        }

        auto const pvMovements = m_MovementProgram->movements();

        if (pvMovements)
        {
            Serial.println("  Steps:");

            for (auto const pMovement : *pvMovements)
            {
                switch (pMovement->type())
                {
                    case Flatbuffers::Firmware::MovementType::Move:
                        Serial.printlnf("    Move: to %d steps, speed %d steps/sec, acceleration %d steps/sec^2",
                                        pMovement->desiredPosition(),
                                        pMovement->desiredSpeed(),
                                        pMovement->desiredAcceleration());
                        break;

                    case Flatbuffers::Firmware::MovementType::Delay:
                        Serial.printlnf("    Delay: %u msec", pMovement->delayTime());
                        break;

                    default:
                        Serial.printlnf("    UnknownMovementType(%u)", pMovement->type());
                        break;
                }
            }
        }
    }
};
