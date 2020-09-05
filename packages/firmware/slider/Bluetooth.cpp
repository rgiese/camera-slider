#include "inc/stdinc.h"

Bluetooth g_Bluetooth;

void Bluetooth::begin()
{
    BLE.on();
    BLE.addCharacteristic(m_StatusService.m_StateCharacteristic);

    BleAdvertisingData advertisingData;
    advertisingData.appendServiceUUID(m_StatusService.m_ServiceUuid);

    BLE.advertise(&advertisingData);
}