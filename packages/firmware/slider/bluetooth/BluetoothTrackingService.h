#pragma once

#include <Particle.h>

class BluetoothTrackingService
{
public:
    BluetoothTrackingService();

private:
    friend class Bluetooth;

    void begin(BleAdvertisingData& advertisingData);

private:
    static void onDesiredPositionChanged(uint8_t const* const pData,
                                         size_t const cbData,
                                         BlePeerDevice const& peerDevice,
                                         void* pContext);

    static void onDesiredMaximumSpeedChanged(uint8_t const* const pData,
                                             size_t const cbData,
                                             BlePeerDevice const& peerDevice,
                                             void* pContext);

    static void onDesiredMaximumAccelerationChanged(uint8_t const* const pData,
                                                    size_t const cbData,
                                                    BlePeerDevice const& peerDevice,
                                                    void* pContext);

private:
    BleCharacteristic m_DesiredPositionCharacteristic;
    BleCharacteristic m_DesiredMaximumSpeedCharacteristic;
    BleCharacteristic m_DesiredMaximumAccelerationCharacteristic;

private:
    // Non-copyable
    BluetoothTrackingService(BluetoothTrackingService const&) = delete;
    BluetoothTrackingService& operator=(BluetoothTrackingService const&) = delete;
};