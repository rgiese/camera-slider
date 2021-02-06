#include "inc/stdinc.h"
#include "generated/bluetoothIds.h"

BluetoothStatusService::BluetoothStatusService()
    : m_StateCharacteristic("state",
                            BleCharacteristicProperty::READ | BleCharacteristicProperty::NOTIFY,
                            BluetoothIds::Status::Characteristics::State,
                            BluetoothIds::Status::Id)
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
    , m_ReportedMovementProgram("reportedMovementProgram",
                                BleCharacteristicProperty::READ | BleCharacteristicProperty::NOTIFY,
                                BluetoothIds::Status::Characteristics::ReportedMovementProgram,
                                BluetoothIds::Status::Id)
{
}

void BluetoothStatusService::begin(BleAdvertisingData& advertisingData)
{
    BLE.addCharacteristic(m_StateCharacteristic);
    BLE.addCharacteristic(m_ReportedPosition);
    BLE.addCharacteristic(m_ReportedVelocity);
    BLE.addCharacteristic(m_ReportedMaximumSpeed);
    BLE.addCharacteristic(m_ReportedMaximumAcceleration);
    BLE.addCharacteristic(m_ReportedMovementProgram);

    advertisingData.appendServiceUUID(BluetoothIds::Status::Id);

    g_StateKeeper.CurrentSliderState.attach(
        [this](SliderState const sliderState) { m_StateCharacteristic.setValue(getSliderStateName(sliderState)); });

    g_MotorController.CurrentPosition.attach([this](int32_t const position) { m_ReportedPosition.setValue(position); });

    g_MotorController.CurrentVelocity.attach([this](int32_t const velocity) { m_ReportedVelocity.setValue(velocity); });

    g_MotorController.MaximumSpeed.attach(
        [this](uint32_t const maximumSpeed) { m_ReportedMaximumSpeed.setValue(maximumSpeed); });

    g_MotorController.MaximumAcceleration.attach(
        [this](uint32_t const maximumAcceleration) { m_ReportedMaximumAcceleration.setValue(maximumAcceleration); });

    g_MovementProgramStore.CurrentMovementProgram.attach([this](MovementProgram const& movementProgram) {
        flatbuffers::FlatBufferBuilder flatbufferBuilder(512);
        {
            movementProgram.toFlatbufferData(flatbufferBuilder);
        }

        m_ReportedMovementProgram.setValue(flatbufferBuilder.GetBufferPointer(), flatbufferBuilder.GetSize());
    });
}
