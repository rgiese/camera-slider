#include "inc/stdinc.h"
#include "generated/bluetoothIds.h"

BluetoothCapabilitiesService::BluetoothCapabilitiesService()
    : m_MaximumSpeedCharacteristic("maxSpeed",
                                   BleCharacteristicProperty::READ | BleCharacteristicProperty::NOTIFY,
                                   BluetoothIds::Capabilities::Characteristics::MaximumSpeed,
                                   BluetoothIds::Capabilities::Id)
    , m_MaximumAccelerationCharacteristic("maxAcceleration",
                                          BleCharacteristicProperty::READ | BleCharacteristicProperty::NOTIFY,
                                          BluetoothIds::Capabilities::Characteristics::MaximumAcceleration,
                                          BluetoothIds::Capabilities::Id)
    , m_MaximumPositionCharacteristic("maxPosition",
                                      BleCharacteristicProperty::READ | BleCharacteristicProperty::NOTIFY,
                                      BluetoothIds::Capabilities::Characteristics::MaximumPosition,
                                      BluetoothIds::Capabilities::Id)
{
}

void BluetoothCapabilitiesService::begin(BleAdvertisingData& advertisingData)
{
    BLE.addCharacteristic(m_MaximumSpeedCharacteristic);
    BLE.addCharacteristic(m_MaximumAccelerationCharacteristic);
    BLE.addCharacteristic(m_MaximumPositionCharacteristic);
}

void BluetoothCapabilitiesService::setMaximumSpeed(uint32_t const stepsPerSec)
{
    m_MaximumSpeedCharacteristic.setValue(stepsPerSec);
}

void BluetoothCapabilitiesService::setMaximumAcceleration(uint32_t const stepsPerSecPerSec)
{
    m_MaximumAccelerationCharacteristic.setValue(stepsPerSecPerSec);
}

void BluetoothCapabilitiesService::setMaximumPosition(int32_t const position)
{
    m_MaximumPositionCharacteristic.setValue(position);
}
