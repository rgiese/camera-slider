#pragma once

class MotorInitializedState : public AbstractState
{
public:
    char const* getName() override
    {
        return "motorInitialized";
    }

    void onEnteringState() override;
    void onLoop() override;
};
