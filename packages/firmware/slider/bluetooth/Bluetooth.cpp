#include "inc/stdinc.h"

Bluetooth g_Bluetooth;

void Bluetooth::begin()
{
    BLE.on();
    m_StatusService.begin();

    BleAdvertisingData advertisingData;
    advertisingData.appendServiceUUID(m_StatusService.m_ServiceUuid);

    BLE.advertise(&advertisingData);
}