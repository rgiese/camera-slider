#include "inc/stdinc.h"

MovementProgramStore g_MovementProgramStore;

MovementProgramStore::MovementProgramStore()
{
    CurrentMovementProgram.attach(
        [](MovementProgram const&) { g_RequestQueue.push({Type : RequestType::UpdatedMovementProgram}); });
}
