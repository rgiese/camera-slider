#pragma once

#include <Particle.h>

class BluetoothProgramService
{
public:
    BluetoothProgramService();

private:
    friend class Bluetooth;

    void begin(BleAdvertisingData& advertisingData);

private:
    static void onDesiredMovementProgramChanged(uint8_t const* const pData,
                                                size_t const cbData,
                                                BlePeerDevice const& peerDevice,
                                                void* pContext);

    static void onStartMovementProgramChanged(uint8_t const* const pData,
                                              size_t const cbData,
                                              BlePeerDevice const& peerDevice,
                                              void* pContext);

    static void onStopMovementProgramChanged(uint8_t const* const pData,
                                             size_t const cbData,
                                             BlePeerDevice const& peerDevice,
                                             void* pContext);

private:
    BleCharacteristic m_DesiredMovementProgram;
    BleCharacteristic m_StartMovementProgram;
    BleCharacteristic m_StopMovementProgram;

private:
    // Non-copyable
    BluetoothProgramService(BluetoothProgramService const&) = delete;
    BluetoothProgramService& operator=(BluetoothProgramService const&) = delete;
};