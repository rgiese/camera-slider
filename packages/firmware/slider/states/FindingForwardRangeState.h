#pragma once

class FindingForwardRangeState : public AbstractState
{
public:
    char const* getName() override
    {
        return "findingForwardRange";
    }

    void onEnteringState() override;
    void onLoop() override;

private:
    int32_t m_LatestCertainPosition;
};
