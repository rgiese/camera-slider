#include "../inc/stdinc.h"
#include "states.h"

StateKeeper g_StateKeeper;

void StateKeeper::onLoop()
{
    if (m_NextState)
    {
        if (m_CurrentState)
        {
            m_CurrentState->onExitingState();
        }

        m_CurrentState.reset(m_NextState.release());

        if (m_CurrentState)
        {
            m_CurrentState->onEnteringState();
        }

        // Update subscribers
        {
            char const* const stateName = m_CurrentState ? m_CurrentState->getName() : "<none>";

            Serial.printlnf("-> State: %s", stateName);
            g_Bluetooth.statusService().setState(stateName);
        }
    }
    else
    {
        if (m_CurrentState)
        {
            m_CurrentState->onLoop();
        }
    }
}
