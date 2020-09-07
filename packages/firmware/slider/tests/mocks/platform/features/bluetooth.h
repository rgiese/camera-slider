#pragma once

enum class BleCharacteristicProperty : uint8_t
{
    NONE,
    BROADCAST,
    READ,
    WRITE_WO_RSP,
    WRITE,
    NOTIFY,
    INDICATE,
    AUTH_SIGN_WRITES,
    EXTENDED_PROP
};

ENABLE_ENUM_CLASS_BITWISE(BleCharacteristicProperty);

class BleAdvertisingData;
class BlePeerDevice;

typedef void (*BleOnDataReceivedCallback)(const uint8_t* data, size_t len, const BlePeerDevice& peer, void* context);

class BleUuid
{
public:
    BleUuid(const char*){};
};

class BleCharacteristic
{
public:
    BleCharacteristic(const char* desc,
                      particle::EnumFlags<BleCharacteristicProperty> properties,
                      const char* charUuid,
                      const BleUuid& svcUuid,
                      BleOnDataReceivedCallback callback = nullptr,
                      void* context = nullptr){};

    ssize_t setValue(const char* str);
};
