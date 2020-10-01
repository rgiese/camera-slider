#pragma once

#include "../inc/MovementProgramOwner.h"

class RunningMovementProgramState : public AbstractState
{
public:
    RunningMovementProgramState(std::shared_ptr<MovementProgramOwner> spMovementProgram)
        : AbstractState(SliderState::RunningMovementProgram)
        , m_spMovementProgram(spMovementProgram)
        , m_idxCurrentStep(static_cast<size_t>(-1))
        , m_DelayStart_msec(0)
    {
    }

    void onEnteringState() override;
    void onLoop() override;

    bool onRequest(Request const& request) override;

private:
    void enterStep(size_t const idxStep);
    void nextStep();
    void exitProgram();

private:
    std::shared_ptr<MovementProgramOwner> m_spMovementProgram;

    size_t m_idxCurrentStep;
    unsigned long m_DelayStart_msec;
};
