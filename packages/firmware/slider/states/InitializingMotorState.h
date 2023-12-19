#pragma once

class InitializingMotorState : public AbstractState
{
public:
    InitializingMotorState() : AbstractState(SliderState::InitializingMotor)
    {
    }

    void onEnteringState() override;
    void onLoop() override;
};
