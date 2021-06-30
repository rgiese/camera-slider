#pragma once

#include <Particle.h>

class BluetoothCapabilitiesService
{
public:
    BluetoothCapabilitiesService();

    void setMaximumPosition(int32_t const position);
    void setMaximumSpeed(uint32_t const stepsPerSec);
    void setMaximumAcceleration(uint32_t const stepsPerSecPerSec);

private:
    friend class Bluetooth;

    void begin(BleAdvertisingData& advertisingData);

private:
    static void onStartTouchscreenCalibrationChanged(uint8_t const* const pData,
                                                     size_t const cbData,
                                                     BlePeerDevice const& peerDevice,
                                                     void* pContext);

private:
    BleCharacteristic m_MaximumPositionCharacteristic;
    BleCharacteristic m_MaximumSpeedCharacteristic;
    BleCharacteristic m_MaximumAccelerationCharacteristic;
    BleCharacteristic m_StartTouchscreenCalibrationCharacteristic;
    BleCharacteristic m_TouchscreenCalibrationDataCharacteristic;

private:
    // Non-copyable
    BluetoothCapabilitiesService(BluetoothCapabilitiesService const&) = delete;
    BluetoothCapabilitiesService& operator=(BluetoothCapabilitiesService const&) = delete;
};