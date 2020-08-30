#pragma once

class MotorFindingForwardRangeState : public AbstractState
{
public:
    char const* getName() override
    {
        return "motorFindingForwardRange";
    }

    void onEnteringState() override;
    void onLoop() override;

private:
    int32_t m_LatestCertainPosition;
};
