#pragma once

class MotorHomingReverseState : public AbstractState
{
public:
    char const* getName() override
    {
        return "homingMotorReverse";
    }

    void onEnteringState() override;
    void onLoop() override;
};
