#include "inc/stdinc.h"
#include "generated/bluetoothIds.h"

BluetoothProgramService::BluetoothProgramService()
    : m_DesiredMovementProgram("desiredMovementProgram",
                               BleCharacteristicProperty::WRITE_WO_RSP,
                               BluetoothIds::Program::Characteristics::DesiredMovementProgram,
                               BluetoothIds::Program::Id,
                               &BluetoothProgramService::onDesiredMovementProgramChanged,
                               nullptr)
    , m_StartMovementProgram("startMovementProgram",
                             BleCharacteristicProperty::WRITE_WO_RSP,
                             BluetoothIds::Program::Characteristics::StartMovementProgram,
                             BluetoothIds::Program::Id,
                             &BluetoothProgramService::onStartMovementProgramChanged,
                             nullptr)
    , m_StopMovementProgram("stopMovementProgram",
                            BleCharacteristicProperty::WRITE_WO_RSP,
                            BluetoothIds::Program::Characteristics::StopMovementProgram,
                            BluetoothIds::Program::Id,
                            &BluetoothProgramService::onStopMovementProgramChanged,
                            nullptr)
{
}

void BluetoothProgramService::begin(BleAdvertisingData& advertisingData)
{
    BLE.addCharacteristic(m_DesiredMovementProgram);
    BLE.addCharacteristic(m_StartMovementProgram);
    BLE.addCharacteristic(m_StopMovementProgram);
}

void BluetoothProgramService::onDesiredMovementProgramChanged(uint8_t const* const pData,
                                                              size_t const cbData,
                                                              BlePeerDevice const& peerDevice,
                                                              void* pContext)
{
    MovementProgram movementProgram;
    {
        if (!MovementProgram::fromFlatbufferData(pData, cbData, movementProgram))
        {
            Serial.println("!! Desired movement program failed to parse.");
            return;
        }

        movementProgram.dump("desired");
    }

    g_MovementProgramStore.setMovementProgram(movementProgram);
}

void BluetoothProgramService::onStartMovementProgramChanged(uint8_t const* const pData,
                                                            size_t const cbData,
                                                            BlePeerDevice const& peerDevice,
                                                            void* pContext)
{
    Request request = {Type : RequestType::StartMovementProgram};
    {
        if (pData || cbData == sizeof(request.StartMovementProgram.atStep))
        {
            memcpy(&request.StartMovementProgram.atStep, pData, sizeof(request.StartMovementProgram.atStep));
        }
    }

    g_RequestQueue.push(request);
}

void BluetoothProgramService::onStopMovementProgramChanged(uint8_t const* const pData,
                                                           size_t const cbData,
                                                           BlePeerDevice const& peerDevice,
                                                           void* pContext)
{
    Request request = {Type : RequestType::StopMovementProgram};
    g_RequestQueue.push(request);
}
