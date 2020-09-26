#pragma once

class HomingState : public AbstractState
{
public:
    HomingState()
        : AbstractState(SliderState::Homing)
    {
    }

    void onEnteringState() override;
    void onLoop() override;
};
