#pragma once

#include <Particle.h>

class BluetoothStatusService
{
public:
    BluetoothStatusService();

    void setState(char const* const stateName);
    void setReportedPosition(int32_t const position);
    void setReportedVelocity(int32_t const velocity);
    void setReportedMaximumSpeed(uint32_t const maximimSpeed);
    void setReportedMaximumAcceleration(uint32_t const maximumAcceleration);

private:
    friend class Bluetooth;

    void begin(BleAdvertisingData& advertisingData);

private:
    BleCharacteristic m_StateCharacteristic;
    BleCharacteristic m_ReportedPositionCharacteristic;
    BleCharacteristic m_ReportedVelocityCharacteristic;
    BleCharacteristic m_ReportedMaximumSpeedCharacteristic;
    BleCharacteristic m_ReportedMaximumAccelerationCharacteristic;

    int32_t m_LastReportedPosition;
    int32_t m_LastReportedVelocity;
    uint32_t m_LastReportedMaximumSpeed;
    uint32_t m_LastReportedMaximumAcceleration;

private:
    // Non-copyable
    BluetoothStatusService(BluetoothStatusService const&) = delete;
    BluetoothStatusService& operator=(BluetoothStatusService const&) = delete;
};