#include "inc/stdinc.h"

MovementProgramStore g_MovementProgramStore;

void MovementProgramStore::setMovementProgram(MovementProgram const& movementProgram)
{
    CurrentMovementProgram.update(movementProgram);

    Request request = {Type : RequestType::UpdatedMovementProgram};

    g_RequestQueue.push(request);
}
