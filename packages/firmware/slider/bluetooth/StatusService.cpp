#include "inc/stdinc.h"

BluetoothStatusService::BluetoothStatusService()
    : m_ServiceUuid("fcccbeb7-eb63-4726-9315-e198b1e5ec1c")
    , m_StateCharacteristic("state",
                            BleCharacteristicProperty::READ | BleCharacteristicProperty::NOTIFY,
                            "dc5d99b0-303c-45c2-b7a2-af6baadc0388",
                            m_ServiceUuid)
    , m_ReportedPositionCharacteristic("reportedPosition",
                                       BleCharacteristicProperty::READ | BleCharacteristicProperty::NOTIFY,
                                       "0a5f29a0-5184-4961-85bc-8aa78ec98bd1",
                                       m_ServiceUuid)
    , m_DesiredPositionCharacteristic("desiredPosition",
                                      BleCharacteristicProperty::WRITE_WO_RSP,
                                      "28449837-bc47-489a-bbda-215a9d744728",
                                      m_ServiceUuid,
                                      &BluetoothStatusService::onDesiredPositionChanged,
                                      nullptr)
    , m_LastReportedPosition(-1)
{
}

void BluetoothStatusService::begin()
{
    BLE.addCharacteristic(m_StateCharacteristic);
    BLE.addCharacteristic(m_ReportedPositionCharacteristic);
    BLE.addCharacteristic(m_DesiredPositionCharacteristic);
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
