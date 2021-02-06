#pragma once

#include <Tic.h>

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
    // Observable state (refreshed in onLoop())
    //

    //
    // For general use
    //

    // Getters
    Observable<TicOperationState> OperationState;
    Observable<bool> IsPositionCertain;
    Observable<int32_t> CurrentPosition;
    Observable<int32_t> CurrentVelocity;
    Observable<uint32_t> MaximumSpeed;
    Observable<uint32_t> MaximumAcceleration;

    bool isHomingActive()
    {
        // Not worth caching
        return m_Tic.getHomingActive();
    }

    void deliverObservables()
    {
        OperationState.deliver();
        IsPositionCertain.deliver();
        CurrentPosition.deliver();
        CurrentVelocity.deliver();
        MaximumSpeed.deliver();
        MaximumAcceleration.deliver();
    }

    // Setters
    void setTargetPosition(int32_t const targetPosition);
    void setMaxSpeed(uint32_t const stepsPerSecond);
    void setMaxAcceleration(uint32_t const stepsPerSecondPerSecond);

    void goHome();
    void safetyStop();

public:
    //
    // Constants
    //

    static constexpr uint32_t c_MaxSafeHomingSpeed_StepsPerSec = 2000;
    static constexpr uint32_t c_DefaultSpeed_StepsPerSec = 8000;
    static constexpr uint32_t c_MaxSafeSpeed_StepsPerSec = 15000;

    static constexpr uint32_t c_DefaultAcceleration_StepsPerSecPerSec = 10000;
    static constexpr uint32_t c_MaxSafeAcceleration_StepsPerSecPerSec = 75000;

private:
    TicI2C m_Tic;

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

    static int32_t velocityFromTicUnits(int32_t const stepsPer10KSec)
    {
        // Return steps per second
        return stepsPer10KSec / 10000;
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
