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

    constexpr RGBColor multiply(float const ratioRed, float const ratioGreen, float const ratioBlue) const
    {
        return RGBColor{clamp<uint8_t>(static_cast<uint8_t>((Red * ratioRed) + 0.5), 0, 255),
                        clamp<uint8_t>(static_cast<uint8_t>((Green * ratioGreen) + 0.5), 0, 255),
                        clamp<uint8_t>(static_cast<uint8_t>((Blue * ratioBlue) + 0.5), 0, 255)};
    };

    constexpr RGBColor multiply(float const ratio) const
    {
        return multiply(ratio, ratio, ratio);
    };

    bool constexpr operator==(RGBColor const& other) const
    {
        return this->Value == other.Value;
    }

    bool constexpr operator!=(RGBColor const& other) const
    {
        return this->Value != other.Value;
    }
};

namespace Colors
{
    constexpr RGBColor Black = RGBColor{0, 0, 0};
    constexpr RGBColor White = RGBColor{0xFF, 0xFF, 0xFF};
    constexpr RGBColor Red = RGBColor{0xFF, 0x00, 0x00};
    constexpr RGBColor Green = RGBColor{0x00, 0xFF, 0x00};
    constexpr RGBColor Blue = RGBColor{0x00, 0x00, 0xFF};

    constexpr RGBColor Disabled = Blue.multiply(0.55f);
}  // namespace Colors