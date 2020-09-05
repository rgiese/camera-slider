#pragma once

#include <Particle.h>

class Bluetooth;

class BluetoothStatusService
{
public:
    BluetoothStatusService()
        : m_ServiceUuid("fcccbeb7-eb63-4726-9315-e198b1e5ec1c")
        , m_StateCharacteristic(
              "state", BleCharacteristicProperty::READ, "dc5d99b0-303c-45c2-b7a2-af6baadc0388", m_ServiceUuid)
    {
    }

    void setState(char const* const stateName)
    {
        m_StateCharacteristic.setValue(stateName);
    }

private:
    friend class Bluetooth;

    BleUuid const m_ServiceUuid;
    BleCharacteristic m_StateCharacteristic;
};

class Bluetooth
{
public:
    Bluetooth() = default;

    void begin();

    BluetoothStatusService& statusService()
    {
        return m_StatusService;
    }

private:
    BluetoothStatusService m_StatusService;

private:
    // Non-copyable
    Bluetooth(Bluetooth const&) = delete;
    Bluetooth& operator=(Bluetooth const&) = delete;
};

extern Bluetooth g_Bluetooth;