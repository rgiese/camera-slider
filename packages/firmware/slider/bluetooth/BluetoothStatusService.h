#pragma once

#include <Particle.h>

class BluetoothStatusService
{
public:
    BluetoothStatusService();

    void setState(char const* const stateName);
    void onStateMachineThreadLoop();
    void onMainThreadLoop();

private:
    friend class Bluetooth;

    void begin(BleAdvertisingData& advertisingData);

private:
    enum RateLimitedCharacteristic
    {
        // Must match initializer order in constructor
        RateLimitedCharacteristic_ReportedPosition,
        RateLimitedCharacteristic_ReportedVelocity,
        RateLimitedCharacteristic_ReportedMaximumSpeed,
        RateLimitedCharacteristic_ReportedMaximumAcceleration,
        RateLimitedCharacteristic__count
    };

private:
    //
    // Non-rate-limited characteristics (updated immediately)
    //
    BleCharacteristic m_StateCharacteristic;

    //
    // Rate-limited characteristics
    //

    // Non-trivial values
    BleCharacteristic m_RateLimitedReportedMovementProgram;
    MovementProgram m_MovementProgramReported;
    uint32_t m_MovementProgramVersionReported;
    uint32_t m_MovementProgramVersionSent;

    // uint32_t values
    BleCharacteristic m_RateLimitedCharacteristics[RateLimitedCharacteristic__count];
    uint32_t m_RateLimitedValuesReported[RateLimitedCharacteristic__count];
    uint32_t m_RateLimitedValuesSent[RateLimitedCharacteristic__count];

    uint32_t m_LastUpdateTime_msec;

private:
    // Non-copyable
    BluetoothStatusService(BluetoothStatusService const&) = delete;
    BluetoothStatusService& operator=(BluetoothStatusService const&) = delete;
};