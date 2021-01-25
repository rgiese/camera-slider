#pragma once

class MovementProgramStore
{
public:
    MovementProgramStore() = default;

    MovementProgram const& getMovementProgram()
    {
        return m_MovementProgram;
    }

    uint32_t getMovementProgramVersion()

    {
        return m_MovementProgramVersion;
    }

    void setMovementProgram(MovementProgram const& movementProgram);

private:
    // Non-copyable
    MovementProgramStore(MovementProgramStore const&) = delete;
    MovementProgramStore& operator=(MovementProgramStore const&) = delete;

private:
    MovementProgram m_MovementProgram;
    uint32_t m_MovementProgramVersion = 0;
};

extern MovementProgramStore g_MovementProgramStore;