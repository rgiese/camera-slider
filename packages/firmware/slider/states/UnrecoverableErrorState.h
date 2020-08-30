#pragma once

class UnrecoverableErrorState : public AbstractState
{
public:
    char const* getName() override
    {
        return "unrecoverableError";
    }

    UnrecoverableErrorState()
    {
    }

    void onEnteringState() override;
    void onLoop() override;
};
