#pragma once

class UnrecoverableErrorState : public AbstractState
{
public:
    UnrecoverableErrorState(char const* format, ...);

    void onEnteringState() override;
    void onLoop() override;

private:
    char m_ErrorMessage[64];
};
