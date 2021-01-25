#include "inc/stdinc.h"

MovementProgramStore g_MovementProgramStore;

void MovementProgramStore::setMovementProgram(MovementProgram const& movementProgram)
{
    m_MovementProgram = movementProgram;
    ++m_MovementProgramVersion;

    Request request = {Type : RequestType::UpdatedMovementProgram};

    g_RequestQueue.push(request);
}
