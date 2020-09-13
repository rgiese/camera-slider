#pragma once

#include <Particle.h>

class BluetoothStatusService
{
public:
    BluetoothStatusService();

    void setState(char const* const stateName);
    void setReportedPosition(int32_t const position);

private:
    friend class Bluetooth;

    void begin(BleAdvertisingData& advertisingData);

private:
    static void onDesiredPositionChanged(uint8_t const* const pData,
                                         size_t const cbData,
                                         BlePeerDevice const& peerDevice,
                                         void* pContext);

private:
    BleCharacteristic m_StateCharacteristic;
    BleCharacteristic m_ReportedPositionCharacteristic;
    BleCharacteristic m_DesiredPositionCharacteristic;

    int32_t m_LastReportedPosition;

private:
    // Non-copyable
    BluetoothStatusService(BluetoothStatusService const&) = delete;
    BluetoothStatusService& operator=(BluetoothStatusService const&) = delete;
};