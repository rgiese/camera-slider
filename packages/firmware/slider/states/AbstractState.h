#pragma once

class AbstractState
{
public:
    AbstractState(){};
    virtual ~AbstractState(){};

    // Debugging tools (must implement)
    virtual char const* getName() = 0;

    // All handlers (optional to implement)
    virtual void onUIButtonPressed(){};

protected:
    friend class StateKeeper;

    virtual void onEnteringState(){};
    virtual void onLoop(){};
    virtual void onExitingState(){};

private:
    // Non-copyable
    AbstractState(AbstractState const&) = delete;
    AbstractState& operator=(AbstractState const&) = delete;
};