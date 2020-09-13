#pragma once

#include <Particle.h>

class BluetoothCapabilitiesService
{
public:
    BluetoothCapabilitiesService();

private:
    friend class Bluetooth;

    void begin(BleAdvertisingData& advertisingData);

private:
    BleCharacteristic m_MaximumSpeedCharacteristic;
    BleCharacteristic m_MaximumAccelerationCharacteristic;
    BleCharacteristic m_MaximumPositionCharacteristic;

private:
    // Non-copyable
    BluetoothCapabilitiesService(BluetoothCapabilitiesService const&) = delete;
    BluetoothCapabilitiesService& operator=(BluetoothCapabilitiesService const&) = delete;
};