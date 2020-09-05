#pragma once

class MotorController
{
public:
    MotorController()
        : m_Tic(14 /* default I2C address */){};

    //
    // For use by Main.cpp
    //

    void begin();
    void onLoop();

    void dumpState();

    //
    // For startup
    //

    bool tryInitialize();

    //
    // For general use
    //

    // Getters
    TicOperationState getOperationState() const
    {
        return m_OperationState;
    }

    int32_t getCurrentPosition() const
    {
        return m_CurrentPosition;
    }

    bool isPositionCertain() const
    {
        return m_IsPositionCertain;
    }

    bool isHomingActive()
    {
        // Not worth caching
        return m_Tic.getHomingActive();
    }

    // Setters
    void setTargetPosition(int32_t const targetPosition);
    void setMaxSpeed(uint32_t const stepsPerSecond);

    void goHome();

public:
    //
    // Constants
    //

    static constexpr uint32_t c_MaxSafeHomingSpeed_StepsPerSec = 2000;

private:
    TicI2C m_Tic;

    // Refreshed in onLoop():
    TicOperationState m_OperationState;
    int32_t m_CurrentPosition;
    bool m_IsPositionCertain;

private:
    static uint32_t accelerationFromTicUnits(uint32_t const stepsPerSecPer100Sec)
    {
        // Return steps per second per second
        return stepsPerSecPer100Sec / 100;
    }

    static uint32_t accelerationToTicUnits(uint32_t const stepsPerSecPerSec)
    {
        // Return steps per second per 100 seconds
        return stepsPerSecPerSec * 100;
    }

    static uint32_t speedFromTicUnits(uint32_t const stepsPer10KSec)
    {
        // Return steps per second
        return stepsPer10KSec / 10000;
    }

    static uint32_t speedToTicUnits(uint32_t const stepsPerSec)
    {
        // Return steps per second per 10K seconds
        return stepsPerSec * 10000;
    }


private:
    // Non-copyable
    MotorController(MotorController const&) = delete;
    MotorController& operator=(MotorController const&) = delete;
};

extern MotorController g_MotorController;
