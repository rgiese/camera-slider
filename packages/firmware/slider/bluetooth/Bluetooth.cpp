#include "inc/stdinc.h"

Bluetooth g_Bluetooth;

void Bluetooth::begin()
{
    BLE.on();

    BleAdvertisingData advertisingData;
    m_CapabilitiesService.begin(advertisingData);
    m_ProgramService.begin(advertisingData);
    m_StatusService.begin(advertisingData);
    m_TrackingService.begin(advertisingData);

    BLE.advertise(&advertisingData);
}