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
