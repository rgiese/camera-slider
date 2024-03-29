#pragma once

class RunningMovementProgramState : public AbstractState
{
public:
    RunningMovementProgramState(uint32_t const desiredInitialStep)
        : AbstractState(SliderState::RunningMovementProgram)
        , m_idxInitialStep(desiredInitialStep)
        , m_CurrentMovementProgram()
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
    uint32_t const m_idxInitialStep;

    MovementProgram m_CurrentMovementProgram;
    size_t m_idxCurrentStep;
    unsigned long m_DelayStart_msec;
};
