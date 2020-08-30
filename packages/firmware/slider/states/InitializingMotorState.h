#pragma once

class InitializingMotorState : public AbstractState
{
public:
    char const* getName() override
    {
        return "initializingMotor";
    }

    void onEnteringState() override;
    void onLoop() override;
};
