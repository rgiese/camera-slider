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
    , m_ReportedVelocityCharacteristic("reportedVelocity",
                                       BleCharacteristicProperty::READ | BleCharacteristicProperty::NOTIFY,
                                       BluetoothIds::Status::Characteristics::ReportedVelocity,
                                       BluetoothIds::Status::Id)
    , m_ReportedMaximumSpeedCharacteristic("reportedMaximumSpeed",
                                           BleCharacteristicProperty::READ | BleCharacteristicProperty::NOTIFY,
                                           BluetoothIds::Status::Characteristics::ReportedMaximumSpeed,
                                           BluetoothIds::Status::Id)
    , m_ReportedMaximumAccelerationCharacteristic("reportedMaximumAcceleration",
                                                  BleCharacteristicProperty::READ | BleCharacteristicProperty::NOTIFY,
                                                  BluetoothIds::Status::Characteristics::ReportedMaximumAcceleration,
                                                  BluetoothIds::Status::Id)
    , m_LastReportedPosition(-1)
    , m_LastReportedVelocity(-1)
    , m_LastReportedMaximumSpeed(static_cast<uint32_t>(-1))
    , m_LastReportedMaximumAcceleration(static_cast<uint32_t>(-1))
{
}

void BluetoothStatusService::begin(BleAdvertisingData& advertisingData)
{
    BLE.addCharacteristic(m_StateCharacteristic);
    BLE.addCharacteristic(m_ReportedPositionCharacteristic);
    BLE.addCharacteristic(m_ReportedVelocityCharacteristic);
    BLE.addCharacteristic(m_ReportedMaximumSpeedCharacteristic);
    BLE.addCharacteristic(m_ReportedMaximumAccelerationCharacteristic);

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

void BluetoothStatusService::setReportedVelocity(int32_t const velocity)
{
    if (velocity != m_LastReportedVelocity)
    {
        m_ReportedVelocityCharacteristic.setValue(velocity);
        m_LastReportedVelocity = velocity;
    }
}

void BluetoothStatusService::setReportedMaximumSpeed(uint32_t const maximumSpeed)
{
    if (maximumSpeed != m_LastReportedMaximumSpeed)
    {
        m_ReportedMaximumSpeedCharacteristic.setValue(maximumSpeed);
        m_LastReportedMaximumSpeed = maximumSpeed;
    }
}

void BluetoothStatusService::setReportedMaximumAcceleration(uint32_t const maximumAcceleration)
{
    if (maximumAcceleration != m_LastReportedMaximumAcceleration)
    {
        m_ReportedMaximumAccelerationCharacteristic.setValue(maximumAcceleration);
        m_LastReportedMaximumAcceleration = maximumAcceleration;
    }
}
