#include "inc/stdinc.h"
#include "generated/bluetoothIds.h"

BluetoothStatusService::BluetoothStatusService()
    : m_StateCharacteristic("state",
                            BleCharacteristicProperty::READ | BleCharacteristicProperty::NOTIFY,
                            BluetoothIds::Status::Characteristics::State,
                            BluetoothIds::Status::Id)
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
    , m_LastUpdatedCharacteristic(RateLimitedCharacteristic__count)
{
}

void BluetoothStatusService::begin(BleAdvertisingData& advertisingData)
{
    BLE.addCharacteristic(m_StateCharacteristic);

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

void BluetoothStatusService::onLoop()
{
    // Update reported values from motor controller
    m_RateLimitedValuesReported[RateLimitedCharacteristic_ReportedPosition] = g_MotorController.getCurrentPosition();
    m_RateLimitedValuesReported[RateLimitedCharacteristic_ReportedVelocity] = g_MotorController.getVelocity();
    m_RateLimitedValuesReported[RateLimitedCharacteristic_ReportedMaximumSpeed] = g_MotorController.getMaximumSpeed();
    m_RateLimitedValuesReported[RateLimitedCharacteristic_ReportedMaximumSpeed] =
        g_MotorController.getMaximumAcceleration();

    // Check whether we should update
    // - Perform rotating/rolling update to distribute interrupt-driven disruptions evenly across time slices
    constexpr uint32_t maxTimeSinceLastFullUpdate_msec = 250;

    constexpr uint32_t maxTimeSinceLastRollingUpdate_msec =
        maxTimeSinceLastFullUpdate_msec / RateLimitedCharacteristic__count;

    uint32_t currentTime_msec = millis();

    if (currentTime_msec - m_LastUpdateTime_msec < maxTimeSinceLastRollingUpdate_msec)
    {
        return;
    }

    // Update
    uint32_t const idxCharacteristicToUpdate = (m_LastUpdatedCharacteristic + 1) % RateLimitedCharacteristic__count;

    if (m_RateLimitedValuesReported[idxCharacteristicToUpdate] != m_RateLimitedValuesSent[idxCharacteristicToUpdate])
    {
        m_RateLimitedValuesSent[idxCharacteristicToUpdate] = m_RateLimitedValuesReported[idxCharacteristicToUpdate];
        m_RateLimitedCharacteristics[idxCharacteristicToUpdate].setValue(
            m_RateLimitedValuesSent[idxCharacteristicToUpdate]);
    }

    // Commit
    m_LastUpdateTime_msec = currentTime_msec;
    m_LastUpdatedCharacteristic = static_cast<RateLimitedCharacteristic>(idxCharacteristicToUpdate);
}