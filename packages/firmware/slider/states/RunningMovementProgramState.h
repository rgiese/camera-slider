#pragma once

#include "../inc/MovementProgramOwner.h"

class RunningMovementProgramState : public AbstractState
{
public:
    RunningMovementProgramState(std::shared_ptr<MovementProgramOwner> spMovementProgram)
        : AbstractState(SliderState::RunningMovementProgram)
        , m_spMovementProgram(spMovementProgram)
    {
    }

    void onEnteringState() override;
    void onLoop() override;

    bool onRequest(Request const& request) override;

private:
    std::shared_ptr<MovementProgramOwner> m_spMovementProgram;
};
