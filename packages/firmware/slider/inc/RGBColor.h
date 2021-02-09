#pragma once

union RGBColor
{
    struct
    {
        uint8_t Red;
        uint8_t Green;
        uint8_t Blue;
    };
    uint32_t Value;

    constexpr uint16_t to565Color() const
    {
        uint8_t const red5Bits = Red >> 3;
        uint8_t const green6Bits = Green >> 2;
        uint8_t const blue5Bits = Blue >> 3;

        return (red5Bits << (5 + 6)) | (green6Bits << 5) | blue5Bits;
    };

    constexpr RGBColor multiply(float const ratio) const
    {
        return RGBColor{clamp<uint8_t>(static_cast<uint8_t>((Red * ratio) + 0.5), 0, 255),
                        clamp<uint8_t>(static_cast<uint8_t>((Green * ratio) + 0.5), 0, 255),
                        clamp<uint8_t>(static_cast<uint8_t>((Blue * ratio) + 0.5), 0, 255)};
    };
};
