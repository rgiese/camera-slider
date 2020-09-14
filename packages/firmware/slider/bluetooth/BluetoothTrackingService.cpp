#include "inc/stdinc.h"
#include "generated/bluetoothIds.h"

BluetoothTrackingService::BluetoothTrackingService()
    : m_DesiredPositionCharacteristic("desiredPosition",
                                      BleCharacteristicProperty::WRITE_WO_RSP,
                                      BluetoothIds::Tracking::Characteristics::DesiredPosition,
                                      BluetoothIds::Tracking::Id,
                                      &BluetoothTrackingService::onDesiredPositionChanged,
                                      nullptr)
    , m_DesiredMaximumSpeedCharacteristic("desiredMaximumSpeed",
                                          BleCharacteristicProperty::WRITE_WO_RSP,
                                          BluetoothIds::Tracking::Characteristics::DesiredMaximumSpeed,
                                          BluetoothIds::Tracking::Id,
                                          &BluetoothTrackingService::onDesiredMaximumSpeedChanged,
                                          nullptr)
    , m_DesiredMaximumAccelerationCharacteristic("desiredMaximumAcceleration",
                                                 BleCharacteristicProperty::WRITE_WO_RSP,
                                                 BluetoothIds::Tracking::Characteristics::DesiredMaximumAcceleration,
                                                 BluetoothIds::Tracking::Id,
                                                 &BluetoothTrackingService::onDesiredMaximumAccelerationChanged,
                                                 nullptr)
{
}

void BluetoothTrackingService::begin(BleAdvertisingData& advertisingData)
{
    BLE.addCharacteristic(m_DesiredPositionCharacteristic);
    BLE.addCharacteristic(m_DesiredMaximumSpeedCharacteristic);
    BLE.addCharacteristic(m_DesiredMaximumAccelerationCharacteristic);
}

void BluetoothTrackingService::onDesiredPositionChanged(uint8_t const* const pData,
                                                        size_t const cbData,
                                                        BlePeerDevice const& peerDevice,
                                                        void* pContext)
{
    Request request = {Type : RequestType::DesiredPosition};
    {
        if (!pData || cbData != sizeof(request.DesiredPosition.value))
        {
            return;
        }

        memcpy(&request.DesiredPosition.value, pData, sizeof(request.DesiredPosition.value));
    }

    g_RequestQueue.push(request);
}

void BluetoothTrackingService::onDesiredMaximumSpeedChanged(uint8_t const* const pData,
                                                            size_t const cbData,
                                                            BlePeerDevice const& peerDevice,
                                                            void* pContext)
{
    Request request = {Type : RequestType::DesiredMaximumSpeed};
    {
        if (!pData || cbData != sizeof(request.DesiredMaximumSpeed.value))
        {
            return;
        }

        memcpy(&request.DesiredMaximumSpeed.value, pData, sizeof(request.DesiredMaximumSpeed.value));
    }

    g_RequestQueue.push(request);
}

void BluetoothTrackingService::onDesiredMaximumAccelerationChanged(uint8_t const* const pData,
                                                                   size_t const cbData,
                                                                   BlePeerDevice const& peerDevice,
                                                                   void* pContext)
{
    Request request = {Type : RequestType::DesiredMaximumAcceleration};
    {
        if (!pData || cbData != sizeof(request.DesiredMaximumAcceleration.value))
        {
            return;
        }

        memcpy(&request.DesiredMaximumAcceleration.value, pData, sizeof(request.DesiredMaximumAcceleration.value));
    }

    g_RequestQueue.push(request);
}
