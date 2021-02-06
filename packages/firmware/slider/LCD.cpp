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

    blitColorRegion(0, 0, m_LCD.width(), m_LCD.height(), HX8357_BLACK);

    m_LCD.setCursor(0, 0);
    m_LCD.setTextColor(HX8357_WHITE);
    m_LCD.setTextSize(1);
    m_LCD.println("Hello World!");
    m_LCD.setTextColor(HX8357_YELLOW);
    m_LCD.setTextSize(2);
    m_LCD.println(1234.56);
    m_LCD.setTextColor(HX8357_RED);
    m_LCD.setTextSize(3);
    m_LCD.println(0xDEADBEEF, HEX);
    m_LCD.setTextColor(HX8357_GREEN);
    m_LCD.setTextSize(5);
    m_LCD.println("Groop");
}

void LCD::blitColorRegion(
    uint16_t const x, uint16_t const y, uint16_t const width, uint16_t const height, uint16_t const color)
{
    // A (much) faster version of m_LCD.fillScreen
    std::array<uint16_t, HX8357_TFTHEIGHT> pixelBuffer;  // about the maximum we're willing to pay in stack space
    pixelBuffer.fill(color);

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
