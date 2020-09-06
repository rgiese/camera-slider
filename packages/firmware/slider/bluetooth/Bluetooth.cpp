#include "inc/stdinc.h"

Bluetooth g_Bluetooth;

void Bluetooth::begin()
{
    BLE.on();

    BleAdvertisingData advertisingData;
    m_StatusService.begin(advertisingData);

    BLE.advertise(&advertisingData);
}