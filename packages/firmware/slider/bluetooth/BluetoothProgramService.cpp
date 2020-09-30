#include "inc/stdinc.h"
#include "generated/bluetoothIds.h"

using namespace Flatbuffers::Firmware;

BluetoothProgramService::BluetoothProgramService()
    : m_DesiredMovementProgram("desiredMovementProgram",
                               BleCharacteristicProperty::WRITE_WO_RSP,
                               BluetoothIds::Program::Characteristics::DesiredMovementProgram,
                               BluetoothIds::Program::Id,
                               &BluetoothProgramService::onDesiredMovementProgramChanged,
                               nullptr)
{
}

void BluetoothProgramService::begin(BleAdvertisingData& advertisingData)
{
    BLE.addCharacteristic(m_DesiredMovementProgram);
}

void BluetoothProgramService::onDesiredMovementProgramChanged(uint8_t const* const pData,
                                                              size_t const cbData,
                                                              BlePeerDevice const& peerDevice,
                                                              void* pContext)
{
    Request request = {Type : RequestType::DesiredMovementProgram};
    {
        request.DesiredMovementProgram.MovementProgram = MovementProgramOwner::Initialize(pData, cbData);

        if (!request.DesiredMovementProgram.MovementProgram)
        {
            // Couldn't verify; don't queue request.
            return;
        }
    }

    g_RequestQueue.push(request);
}
