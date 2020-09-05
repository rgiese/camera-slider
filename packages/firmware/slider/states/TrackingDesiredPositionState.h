#pragma once

class TrackingDesiredPositionState : public AbstractState
{
public:
    char const* getName() override
    {
        return "trackingDesiredPosition";
    }

    void onEnteringState() override;
    void onLoop() override;

    bool onRequest(Request const& request) override;
};
