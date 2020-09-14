#pragma once

#include <Particle.h>

class Bluetooth;

#include "BluetoothCapabilitiesService.h"
#include "BluetoothStatusService.h"
#include "BluetoothTrackingService.h"

class Bluetooth
{
public:
    Bluetooth() = default;

    void begin();

    BluetoothCapabilitiesService& capabilitiesService()
    {
        return m_CapabilitiesService;
    }

    BluetoothStatusService& statusService()
    {
        return m_StatusService;
    }

    BluetoothTrackingService& trackingService()
    {
        return m_TrackingService;
    }

private:
    BluetoothCapabilitiesService m_CapabilitiesService;
    BluetoothStatusService m_StatusService;
    BluetoothTrackingService m_TrackingService;

private:
    // Non-copyable
    Bluetooth(Bluetooth const&) = delete;
    Bluetooth& operator=(Bluetooth const&) = delete;
};

extern Bluetooth g_Bluetooth;