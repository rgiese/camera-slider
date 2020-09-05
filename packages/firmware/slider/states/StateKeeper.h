#pragma once

class StateKeeper
{
public:
    StateKeeper(){};

    // For use by main code to deliver notifications to current state
    AbstractState* CurrentState() const
    {
        return m_CurrentState.get();
    };

    void onLoop();

    // For use by state objects
    void RequestState(AbstractState* const nextState)
    {
        m_NextState.reset(nextState);
    }

private:
    std::unique_ptr<AbstractState> m_CurrentState;
    std::unique_ptr<AbstractState> m_NextState;

private:
    // Non-copyable
    StateKeeper(StateKeeper const&) = delete;
    StateKeeper& operator=(StateKeeper const&) = delete;
};
