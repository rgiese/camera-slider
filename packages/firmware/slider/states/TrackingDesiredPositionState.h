#pragma once

class TrackingDesiredPositionState : public AbstractState
{
public:
    TrackingDesiredPositionState()
        : AbstractState(SliderState::TrackingDesiredPosition)
    {
    }

    void onEnteringState() override;
    void onLoop() override;

    bool onRequest(Request const& request) override;
};
