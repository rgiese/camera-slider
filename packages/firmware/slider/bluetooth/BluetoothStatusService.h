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
    //
    // Immediately updated characteristics
    //
    BleCharacteristic m_StateCharacteristic;

    //
    // Deferred characteristics
    //

    BleCharacteristic m_RateLimitedReportedMovementProgram;
    MovementProgram m_MovementProgramReported;
    uint32_t m_MovementProgramVersionReported;
    uint32_t m_MovementProgramVersionSent;

    BleCharacteristic m_ReportedPosition;
    BleCharacteristic m_ReportedVelocity;
    BleCharacteristic m_ReportedMaximumSpeed;
    BleCharacteristic m_ReportedMaximumAcceleration;

private:
    // Non-copyable
    BluetoothStatusService(BluetoothStatusService const&) = delete;
    BluetoothStatusService& operator=(BluetoothStatusService const&) = delete;
};