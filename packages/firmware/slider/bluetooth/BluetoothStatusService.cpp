#include "inc/stdinc.h"
#include "generated/bluetoothIds.h"

BluetoothStatusService::BluetoothStatusService()
    : m_StateCharacteristic("state",
                            BleCharacteristicProperty::READ | BleCharacteristicProperty::NOTIFY,
                            BluetoothIds::Status::Characteristics::State,
                            BluetoothIds::Status::Id)
    , m_ReportedPositionCharacteristic("reportedPosition",
                                       BleCharacteristicProperty::READ | BleCharacteristicProperty::NOTIFY,
                                       BluetoothIds::Status::Characteristics::ReportedPosition,
                                       BluetoothIds::Status::Id)
    , m_DesiredPositionCharacteristic("desiredPosition",
                                      BleCharacteristicProperty::WRITE_WO_RSP,
                                      BluetoothIds::Status::Characteristics::DesiredPosition,
                                      BluetoothIds::Status::Id,
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

    advertisingData.appendServiceUUID(BluetoothIds::Status::Id);
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
