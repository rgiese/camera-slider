#pragma once

#include <Particle.h>

class Bluetooth;

#include "BluetoothCapabilitiesService.h"
#include "BluetoothStatusService.h"

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

private:
    BluetoothCapabilitiesService m_CapabilitiesService;
    BluetoothStatusService m_StatusService;

private:
    // Non-copyable
    Bluetooth(Bluetooth const&) = delete;
    Bluetooth& operator=(Bluetooth const&) = delete;
};

extern Bluetooth g_Bluetooth;