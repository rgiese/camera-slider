#pragma once

class HomingState : public AbstractState
{
public:
    char const* getName() override
    {
        return "homing";
    }

    void onEnteringState() override;
    void onLoop() override;
};
