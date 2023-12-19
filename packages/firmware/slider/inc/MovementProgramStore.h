#pragma once

class MovementProgramStore
{
public:
    MovementProgramStore();

    Observable<MovementProgram> CurrentMovementProgram;

private:
    // Non-copyable
    MovementProgramStore(MovementProgramStore const&) = delete;
    MovementProgramStore& operator=(MovementProgramStore const&) = delete;
};

extern MovementProgramStore g_MovementProgramStore;