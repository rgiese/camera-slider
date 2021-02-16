#pragma once

class WireTransmission
{
public:
    WireTransmission(uint8_t address){};

    WireTransmission& timeout(std::chrono::milliseconds ms)
    {
        return *this;
    }
};

class TwoWire
{
public:
    TwoWire()
    {
    }

public:
    void beginTransmission(uint8_t const _address)
    {
    }

    void beginTransmission(const WireTransmission&)
    {
    }

    byte endTransmission()
    {
        return 0;
    }

    size_t requestFrom(uint8_t, size_t)
    {
        return 0;
    }

    int available()
    {
        return 0;
    }

    int read()
    {
        return 0;
    }

    void write(uint8_t const)
    {
    }
};

extern TwoWire Wire;
