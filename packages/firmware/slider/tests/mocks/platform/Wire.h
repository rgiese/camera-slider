#pragma once

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

    byte endTransmission()
    {
        return 0;
    }

    void write(uint8_t const _data)
    {
    }
};

extern TwoWire Wire;