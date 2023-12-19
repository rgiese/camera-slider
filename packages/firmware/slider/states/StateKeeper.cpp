#include "../inc/stdinc.h"
#include "states.h"

StateKeeper g_StateKeeper;

void StateKeeper::onLoop()
{
    // Advance state if requested
    if (m_NextState)
    {
        if (m_CurrentState)
        {
            m_CurrentState->onExitingState();
        }

        m_CurrentState.reset(m_NextState.release());

        m_CurrentState->onEnteringState();

        // Update subscribers
        {
            CurrentSliderState.update(m_CurrentState->getSliderState());
            Serial.printlnf("-> State: %s", getSliderStateName(m_CurrentState->getSliderState()));
        }
    }

    // If we don't have a state yet, bail
    if (!m_CurrentState)
    {
        return;
    }

    // Deliver loop notification to current state
    m_CurrentState->onLoop();

    // Deliver requests to current state as long as no state transition has been requested
    Request request;

    while (!m_NextState && g_RequestQueue.try_pop(request))
    {
        bool const wasProcessed = m_CurrentState->onRequest(request);

        if (!wasProcessed)
        {
            Serial.printlnf("!! Request type %u dropped (rejected) by current state %s.",
                            static_cast<int>(request.Type),
                            getSliderStateName(m_CurrentState->getSliderState()));
        }
    }
}
