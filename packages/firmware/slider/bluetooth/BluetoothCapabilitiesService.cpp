#include "inc/stdinc.h"
#include "generated/bluetoothConstants.h"

BluetoothCapabilitiesService::BluetoothCapabilitiesService()
    : m_MaximumSpeedCharacteristic("maxSpeed",
                                   BleCharacteristicProperty::READ | BleCharacteristicProperty::NOTIFY,
                                   BluetoothCapabilitiesServiceConstants::maximumSpeedCharacteristicUuid,
                                   BluetoothCapabilitiesServiceConstants::serviceUuid)
    , m_MaximumAccelerationCharacteristic("maxAcceleration",
                                          BleCharacteristicProperty::READ | BleCharacteristicProperty::NOTIFY,
                                          BluetoothCapabilitiesServiceConstants::maximumAccelerationCharacteristicUuid,
                                          BluetoothCapabilitiesServiceConstants::serviceUuid)
    , m_MaximumPositionCharacteristic("maxPosition",
                                      BleCharacteristicProperty::READ | BleCharacteristicProperty::NOTIFY,
                                      BluetoothCapabilitiesServiceConstants::maximumPositionCharacteristicUuid,
                                      BluetoothCapabilitiesServiceConstants::serviceUuid)
{
}

void BluetoothCapabilitiesService::begin(BleAdvertisingData& advertisingData)
{
    BLE.addCharacteristic(m_MaximumSpeedCharacteristic);
    BLE.addCharacteristic(m_MaximumAccelerationCharacteristic);
    BLE.addCharacteristic(m_MaximumPositionCharacteristic);
}
