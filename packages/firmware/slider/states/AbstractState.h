#pragma once

class AbstractState
{
public:
    AbstractState(SliderState const sliderState) : m_SliderState(sliderState){};

    virtual ~AbstractState(){};

    SliderState getSliderState() const
    {
        return m_SliderState;
    }

    // Request handler (return true if handled)
    virtual bool onRequest(Request const& request)
    {
        return false;
    };

protected:
    friend class StateKeeper;

    virtual void onEnteringState(){};
    virtual void onLoop(){};
    virtual void onExitingState(){};

private:
    SliderState const m_SliderState;

private:
    // Non-copyable
    AbstractState(AbstractState const&) = delete;
    AbstractState& operator=(AbstractState const&) = delete;
};