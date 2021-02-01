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

    auto const startTime = millis();

    {
        // A (much) faster version of m_LCD.fillScreen(HX8357_BLACK)
        m_LCD.startWrite();
        m_LCD.setAddrWindow(0, 0, HX8357_TFTHEIGHT, HX8357_TFTWIDTH);  // switching width/height due to rotation

        uint16_t blackColumn[HX8357_TFTHEIGHT];
        memset(blackColumn, 0, sizeof(blackColumn));

        for (uint16_t idxColumn = 0; idxColumn < HX8357_TFTWIDTH; ++idxColumn)
        {
            SPI.transfer(blackColumn, NULL, sizeof(blackColumn), NULL);
        }

        m_LCD.endWrite();
    }

    auto const endTime = millis();
    auto const blankTime = endTime - startTime;

    m_LCD.setCursor(0, 0);
    m_LCD.setTextColor(HX8357_WHITE);
    m_LCD.setTextSize(1);
    m_LCD.println("Hello World!");
    m_LCD.setTextColor(HX8357_YELLOW);
    m_LCD.setTextSize(2);
    m_LCD.println(1234.56);
    m_LCD.print("Time to blank: ");
    m_LCD.println(blankTime);
    m_LCD.setTextColor(HX8357_RED);
    m_LCD.setTextSize(3);
    m_LCD.println(0xDEADBEEF, HEX);
    m_LCD.println();
    m_LCD.setTextColor(HX8357_GREEN);
    m_LCD.setTextSize(5);
    m_LCD.println("Groop");
}