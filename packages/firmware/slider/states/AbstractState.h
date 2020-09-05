#pragma once

class AbstractState
{
public:
    AbstractState(){};
    virtual ~AbstractState(){};

    // Debugging tools (must implement)
    virtual char const* getName() = 0;

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
    // Non-copyable
    AbstractState(AbstractState const&) = delete;
    AbstractState& operator=(AbstractState const&) = delete;
};