#pragma once

#include <Particle.h>

class BluetoothStatusService
{
public:
    BluetoothStatusService();

private:
    friend class Bluetooth;

    void begin(BleAdvertisingData& advertisingData);

private:
    BleCharacteristic m_StateCharacteristic;
    BleCharacteristic m_ReportedPosition;
    BleCharacteristic m_ReportedVelocity;
    BleCharacteristic m_ReportedMaximumSpeed;
    BleCharacteristic m_ReportedMaximumAcceleration;
    BleCharacteristic m_ReportedMovementProgram;

private:
    // Non-copyable
    BluetoothStatusService(BluetoothStatusService const&) = delete;
    BluetoothStatusService& operator=(BluetoothStatusService const&) = delete;
};