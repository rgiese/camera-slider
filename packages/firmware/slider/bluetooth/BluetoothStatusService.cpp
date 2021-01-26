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
    , m_RateLimitedCharacteristics(
          {BleCharacteristic("reportedPosition",
                             BleCharacteristicProperty::READ | BleCharacteristicProperty::NOTIFY,
                             BluetoothIds::Status::Characteristics::ReportedPosition,
                             BluetoothIds::Status::Id),
           BleCharacteristic("reportedVelocity",
                             BleCharacteristicProperty::READ | BleCharacteristicProperty::NOTIFY,
                             BluetoothIds::Status::Characteristics::ReportedVelocity,
                             BluetoothIds::Status::Id),
           BleCharacteristic("reportedMaximumSpeed",
                             BleCharacteristicProperty::READ | BleCharacteristicProperty::NOTIFY,
                             BluetoothIds::Status::Characteristics::ReportedMaximumSpeed,
                             BluetoothIds::Status::Id),
           BleCharacteristic("reportedMaximumAcceleration",
                             BleCharacteristicProperty::READ | BleCharacteristicProperty::NOTIFY,
                             BluetoothIds::Status::Characteristics::ReportedMaximumAcceleration,
                             BluetoothIds::Status::Id)})
    , m_RateLimitedValuesReported()
    , m_RateLimitedValuesSent()
    , m_LastUpdateTime_msec(0)
{
}

void BluetoothStatusService::begin(BleAdvertisingData& advertisingData)
{
    BLE.addCharacteristic(m_StateCharacteristic);

    BLE.addCharacteristic(m_RateLimitedReportedMovementProgram);

    for (size_t idx = 0; idx < countof(m_RateLimitedCharacteristics); ++idx)
    {
        BLE.addCharacteristic(m_RateLimitedCharacteristics[idx]);
    }

    advertisingData.appendServiceUUID(BluetoothIds::Status::Id);
}

void BluetoothStatusService::setState(char const* const stateName)
{
    m_StateCharacteristic.setValue(stateName);
}

void BluetoothStatusService::onStateMachineThreadLoop()
{
    // Update reported values from motor controller
    m_RateLimitedValuesReported[RateLimitedCharacteristic_ReportedPosition] = g_MotorController.getCurrentPosition();
    m_RateLimitedValuesReported[RateLimitedCharacteristic_ReportedVelocity] = g_MotorController.getVelocity();
    m_RateLimitedValuesReported[RateLimitedCharacteristic_ReportedMaximumSpeed] = g_MotorController.getMaximumSpeed();
    m_RateLimitedValuesReported[RateLimitedCharacteristic_ReportedMaximumAcceleration] =
        g_MotorController.getMaximumAcceleration();

    if (m_MovementProgramVersionReported != g_MovementProgramStore.getMovementProgramVersion())
    {
        m_MovementProgramReported = g_MovementProgramStore.getMovementProgram();
        m_MovementProgramVersionReported = g_MovementProgramStore.getMovementProgramVersion();
    }
}

void BluetoothStatusService::onMainThreadLoop()
{
    // Check whether we should send updates
    constexpr uint32_t maxTimeSinceLastFullUpdate_msec = 100;

    uint32_t currentTime_msec = millis();

    if (currentTime_msec - m_LastUpdateTime_msec < maxTimeSinceLastFullUpdate_msec)
    {
        return;
    }

    // Send updates
    for (size_t idxCharacteristic = 0; idxCharacteristic < countof(m_RateLimitedCharacteristics); ++idxCharacteristic)
    {
        if (m_RateLimitedValuesReported[idxCharacteristic] != m_RateLimitedValuesSent[idxCharacteristic])
        {
            m_RateLimitedValuesSent[idxCharacteristic] = m_RateLimitedValuesReported[idxCharacteristic];
            m_RateLimitedCharacteristics[idxCharacteristic].setValue(m_RateLimitedValuesSent[idxCharacteristic]);
        }
    }

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

    // Commit
    m_LastUpdateTime_msec = currentTime_msec;
}