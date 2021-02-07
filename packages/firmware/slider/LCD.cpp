#include "inc/stdinc.h"

LCD::LCD()
    : m_LCD(D14, D15, D7)
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

void LCD::StaticText::setText(std::string const& text) const
{
    m_Parent.m_LCD.setCursor(m_Rect.X, m_Rect.Y);
    m_Parent.m_LCD.setTextColor(m_Color.to565Color());
    m_Parent.m_LCD.setTextSize(3);
    m_Parent.m_LCD.print(text.c_str());

    Serial.printlnf("... Static: %s at %d, %d, color %x", text.c_str(), m_Rect.X, m_Rect.Y, m_Color.to565Color());
}
