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
    if (!pData || cbData < sizeof(uint32_t))
    {
        return;
    }

    // Validate flatbuffer
    flatbuffers::Verifier verifier(pData, cbData);
    if (!VerifyMovementProgramBuffer(verifier))
    {
        Serial.println("!! Couldn't verify new program flatbuffer: ");
        return;
    }

    // Dump flatbuffer (testing only)
    MovementProgram const* movementProgram = GetMovementProgram(pData);

    Serial.println("Movement program:");

    {
        auto const movementProgramFlags = movementProgram->flags();

        if (static_cast<uint32_t>(movementProgramFlags) != 0)
        {
            Serial.println("  Flags");

            if (!!(movementProgramFlags & MovementProgramFlags::Repeat))
            {
                Serial.println("  - Repeat");
            }
        }

        Serial.printlnf("  Rate: %.2f", static_cast<double>(movementProgram->rate_minus1_x100() - 1) / 100);
    }

    auto const pvMovements = movementProgram->movements();

    if (pvMovements)
    {
        Serial.println("  Steps:");

        for (auto const pMovement : *pvMovements)
        {
            switch (pMovement->type())
            {
                case MovementType::Move:
                    Serial.printlnf("    Move: to %d steps, speed %d steps/sec, acceleration %d steps/sec^2",
                                    pMovement->desiredPosition(),
                                    pMovement->desiredSpeed(),
                                    pMovement->desiredAcceleration());
                    break;

                case MovementType::Delay:
                    Serial.printlnf("    Delay: %u msec", pMovement->delayTime());
                    break;

                default:
                    Serial.printlnf("    UnknownMovementType(%u)", pMovement->type());
                    break;
            }
        }
    }
}
