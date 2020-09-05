#include "inc/stdinc.h"

Display g_Display;

void Display::begin()
{
    m_Display.begin();
}

void Display::set(char const* const szText)
{
    m_Display.clearDisplay();

    m_Display.setTextSize(1);
    m_Display.setTextColor(SSD1306_WHITE);

    m_Display.setCursor(0, 0);
    m_Display.println(szText);

    m_Display.display();

    WITH_LOCK(Serial)
    {
        Serial.print("-- Display update: ");
        Serial.println(szText);
    }
}
