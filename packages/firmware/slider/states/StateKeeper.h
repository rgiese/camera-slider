#pragma once

class StateKeeper
{
public:
    StateKeeper(){};

    void onLoop();

    // For use by state objects
    void RequestState(AbstractState* const nextState)
    {
        if (!nextState)
        {
            return;
        }

        m_NextState.reset(nextState);
    }

    Observable<SliderState> CurrentSliderState;

private:
    std::unique_ptr<AbstractState> m_CurrentState;
    std::unique_ptr<AbstractState> m_NextState;

private:
    // Non-copyable
    StateKeeper(StateKeeper const&) = delete;
    StateKeeper& operator=(StateKeeper const&) = delete;
};
