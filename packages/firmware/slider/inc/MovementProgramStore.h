#pragma once

class MovementProgramStore
{
public:
    MovementProgramStore() = default;

    MovementProgram const& getMovementProgram()
    {
        return m_MovementProgram;
    }

    void setMovementProgram(MovementProgram const& movementProgram);

private:
    // Non-copyable
    MovementProgramStore(MovementProgramStore const&) = delete;
    MovementProgramStore& operator=(MovementProgramStore const&) = delete;

private:
    MovementProgram m_MovementProgram;
};

extern MovementProgramStore g_MovementProgramStore;