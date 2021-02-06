#pragma once

class MovementProgramStore
{
public:
    MovementProgramStore() = default;

    Observable<MovementProgram> CurrentMovementProgram;

    void setMovementProgram(MovementProgram const& movementProgram);

private:
    // Non-copyable
    MovementProgramStore(MovementProgramStore const&) = delete;
    MovementProgramStore& operator=(MovementProgramStore const&) = delete;
};

extern MovementProgramStore g_MovementProgramStore;