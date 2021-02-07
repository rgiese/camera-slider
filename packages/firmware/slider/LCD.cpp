#include "inc/stdinc.h"

LCD::LCD()
    : m_LCD(D14, D15, D7)
    , m_MonochromeCanvas(128, 64)
{
}

void LCD::begin()
{
    // Configure control outputs for LCD
    analogWrite(LCDBrightnessPin, 255);

    m_LCD.begin(32 * 1000 * 1000);  // 32Mhz is the Particle Gen3 maximum
    m_LCD.setRotation(3);

    blitColorRegion(0, 0, m_LCD.width(), m_LCD.height(), RGBColor());
}

void LCD::blitColorRegion(
    uint16_t const x, uint16_t const y, uint16_t const width, uint16_t const height, RGBColor const color)
{
    // A (much) faster version of m_LCD.fillScreen
    std::array<uint16_t, HX8357_TFTHEIGHT> pixelBuffer;  // about the maximum we're willing to pay in stack space
    pixelBuffer.fill(color.to565Color());

    m_LCD.startWrite();
    m_LCD.setAddrWindow(x, y, width, height);

    size_t cPixelsRemaining = width * height;

    while (cPixelsRemaining)
    {
        size_t const cPixelsToSend = std::min(cPixelsRemaining, pixelBuffer.size());
        SPI.transfer(pixelBuffer.data(), NULL, cPixelsToSend * sizeof(uint16_t), NULL);

        cPixelsRemaining -= cPixelsToSend;
    }

    m_LCD.endWrite();
}

void LCD::blitMonochromeCanvas(uint16_t const x,
                               uint16_t const y,
                               uint16_t const width,
                               uint16_t const height,
                               RGBColor const foregroundColor,
                               RGBColor const backgroundColor)
{
    constexpr size_t cMaxPixelsInBuffer = 480;  // ...about the maximum we're willing to pay in stack space
    std::array<uint16_t, cMaxPixelsInBuffer> pixelBuffer;

    m_LCD.startWrite();
    m_LCD.setAddrWindow(x, y, width, height);

    size_t cPixelsInBuffer = 0;

    uint16_t const foregroundColorValue = __bswap_16(foregroundColor.to565Color());
    uint16_t const backgroundColorValue = __bswap_16(backgroundColor.to565Color());

    for (uint16_t sourceY = 0; sourceY < height; ++sourceY)
    {
        for (uint16_t sourceX = 0; sourceX < width; ++sourceX)
        {
            pixelBuffer[cPixelsInBuffer] =
                m_MonochromeCanvas.getPixel(sourceX, sourceY) ? foregroundColorValue : backgroundColorValue;
            ++cPixelsInBuffer;

            if (cPixelsInBuffer == pixelBuffer.size())
            {
                SPI.transfer(pixelBuffer.data(), NULL, cPixelsInBuffer * sizeof(uint16_t), NULL);
                cPixelsInBuffer = 0;
            }
        }
    }

    if (cPixelsInBuffer)
    {
        SPI.transfer(pixelBuffer.data(), NULL, cPixelsInBuffer * sizeof(uint16_t), NULL);
    }

    m_LCD.endWrite();
}

void LCD::StaticText::setText(std::string const& text) const
{
    // Write to backbuffer
    GFXcanvas1& monochromeCanvas = m_Parent.m_MonochromeCanvas;
    {
        if (monochromeCanvas.width() < m_Rect.Width || monochromeCanvas.height() < m_Rect.Height)
        {
            // Consider tiling later
            return;
        }

        monochromeCanvas.fillScreen(0);

        monochromeCanvas.setCursor(0, 0);  // The blitting operation below will position X/Y
        monochromeCanvas.setTextColor(static_cast<uint16_t>(-1));
        monochromeCanvas.setTextSize(3);
        monochromeCanvas.print(text.c_str());
    }

    // Write backbuffer to device
    m_Parent.blitMonochromeCanvas(
        m_Rect.X, m_Rect.Y, m_Rect.Width, m_Rect.Height, m_ForegroundColor, m_BackgroundColor);
}
