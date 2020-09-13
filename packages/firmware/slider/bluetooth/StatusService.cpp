#include "inc/stdinc.h"
#include "generated/bluetoothConstants.h"

BluetoothStatusService::BluetoothStatusService()
    : m_StateCharacteristic("state",
                            BleCharacteristicProperty::READ | BleCharacteristicProperty::NOTIFY,
                            BluetoothStatusServiceConstants::stateCharacteristicUuid,
                            BluetoothStatusServiceConstants::serviceUuid)
    , m_ReportedPositionCharacteristic("reportedPosition",
                                       BleCharacteristicProperty::READ | BleCharacteristicProperty::NOTIFY,
                                       BluetoothStatusServiceConstants::reportedPositionCharacteristicUuid,
                                       BluetoothStatusServiceConstants::serviceUuid)
    , m_DesiredPositionCharacteristic("desiredPosition",
                                      BleCharacteristicProperty::WRITE_WO_RSP,
                                      BluetoothStatusServiceConstants::desiredPositionCharacteristicUuid,
                                      BluetoothStatusServiceConstants::serviceUuid,
                                      &BluetoothStatusService::onDesiredPositionChanged,
                                      nullptr)
    , m_LastReportedPosition(-1)
{
}

void BluetoothStatusService::begin(BleAdvertisingData& advertisingData)
{
    BLE.addCharacteristic(m_StateCharacteristic);
    BLE.addCharacteristic(m_ReportedPositionCharacteristic);
    BLE.addCharacteristic(m_DesiredPositionCharacteristic);

    advertisingData.appendServiceUUID(BluetoothStatusServiceConstants::serviceUuid);
}

void BluetoothStatusService::setState(char const* const stateName)
{
    m_StateCharacteristic.setValue(stateName);
}

void BluetoothStatusService::setReportedPosition(int32_t const position)
{
    if (position != m_LastReportedPosition)
    {
        m_ReportedPositionCharacteristic.setValue(position);
        m_LastReportedPosition = position;
    }
}

void BluetoothStatusService::onDesiredPositionChanged(uint8_t const* const pData,
                                                      size_t const cbData,
                                                      BlePeerDevice const& peerDevice,
                                                      void* pContext)
{
    int32_t desiredPosition;
    {
        if (!pData || cbData != sizeof(desiredPosition))
        {
            return;
        }

        memcpy(&desiredPosition, pData, sizeof(desiredPosition));
    }

    g_RequestQueue.push({Type : RequestType::DesiredPosition, Data : {DesiredPosition : desiredPosition}});
}
