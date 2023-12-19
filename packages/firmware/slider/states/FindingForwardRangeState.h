#pragma once

class FindingForwardRangeState : public AbstractState
{
public:
    FindingForwardRangeState() : AbstractState(SliderState::FindingForwardRange)
    {
    }

    void onEnteringState() override;
    void onLoop() override;

private:
    int32_t m_LatestCertainPosition;
};
