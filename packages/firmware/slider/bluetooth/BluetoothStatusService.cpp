#include "inc/stdinc.h"
#include "generated/bluetoothIds.h"

BluetoothStatusService::BluetoothStatusService()
    : m_StateCharacteristic("state",
                            BleCharacteristicProperty::READ | BleCharacteristicProperty::NOTIFY,
                            BluetoothIds::Status::Characteristics::State,
                            BluetoothIds::Status::Id)
    , m_RateLimitedReportedMovementProgram("reportedMovementProgram",
                                           BleCharacteristicProperty::READ | BleCharacteristicProperty::NOTIFY,
                                           BluetoothIds::Status::Characteristics::ReportedMovementProgram,
                                           BluetoothIds::Status::Id)
    , m_MovementProgramReported()
    , m_MovementProgramVersionReported(static_cast<uint32_t>(-1))
    , m_MovementProgramVersionSent(static_cast<uint32_t>(-1))
    , m_ReportedPosition("reportedPosition",
                         BleCharacteristicProperty::READ | BleCharacteristicProperty::NOTIFY,
                         BluetoothIds::Status::Characteristics::ReportedPosition,
                         BluetoothIds::Status::Id)
    , m_ReportedVelocity("reportedVelocity",
                         BleCharacteristicProperty::READ | BleCharacteristicProperty::NOTIFY,
                         BluetoothIds::Status::Characteristics::ReportedVelocity,
                         BluetoothIds::Status::Id)
    , m_ReportedMaximumSpeed("reportedMaximumSpeed",
                             BleCharacteristicProperty::READ | BleCharacteristicProperty::NOTIFY,
                             BluetoothIds::Status::Characteristics::ReportedMaximumSpeed,
                             BluetoothIds::Status::Id)
    , m_ReportedMaximumAcceleration("reportedMaximumAcceleration",
                                    BleCharacteristicProperty::READ | BleCharacteristicProperty::NOTIFY,
                                    BluetoothIds::Status::Characteristics::ReportedMaximumAcceleration,
                                    BluetoothIds::Status::Id)
{
}

void BluetoothStatusService::begin(BleAdvertisingData& advertisingData)
{
    BLE.addCharacteristic(m_StateCharacteristic);
    BLE.addCharacteristic(m_RateLimitedReportedMovementProgram);
    BLE.addCharacteristic(m_ReportedPosition);
    BLE.addCharacteristic(m_ReportedVelocity);
    BLE.addCharacteristic(m_ReportedMaximumSpeed);
    BLE.addCharacteristic(m_ReportedMaximumAcceleration);

    advertisingData.appendServiceUUID(BluetoothIds::Status::Id);

    g_MotorController.CurrentPosition.attach([this](int32_t const position) { m_ReportedPosition.setValue(position); });

    g_MotorController.CurrentVelocity.attach([this](int32_t const velocity) { m_ReportedVelocity.setValue(velocity); });

    g_MotorController.MaximumSpeed.attach(
        [this](uint32_t const maximumSpeed) { m_ReportedMaximumSpeed.setValue(maximumSpeed); });

    g_MotorController.MaximumAcceleration.attach(
        [this](uint32_t const maximumAcceleration) { m_ReportedMaximumAcceleration.setValue(maximumAcceleration); });
}

void BluetoothStatusService::setState(char const* const stateName)
{
    m_StateCharacteristic.setValue(stateName);
}

void BluetoothStatusService::onStateMachineThreadLoop()
{
    if (m_MovementProgramVersionReported != g_MovementProgramStore.getMovementProgramVersion())
    {
        m_MovementProgramReported = g_MovementProgramStore.getMovementProgram();
        m_MovementProgramVersionReported = g_MovementProgramStore.getMovementProgramVersion();
    }
}

void BluetoothStatusService::onMainThreadLoop()
{
    if (m_MovementProgramVersionReported != m_MovementProgramVersionSent)
    {
        flatbuffers::FlatBufferBuilder flatbufferBuilder(512);
        {
            m_MovementProgramReported.toFlatbufferData(flatbufferBuilder);
        }

        m_RateLimitedReportedMovementProgram.setValue(flatbufferBuilder.GetBufferPointer(),
                                                      flatbufferBuilder.GetSize());

        m_MovementProgramVersionSent = m_MovementProgramVersionReported;
    }
}