#pragma once

namespace Display
{
    namespace
    {
        Adafruit_SSD1306 g_Display = Adafruit_SSD1306(128, 32, &Wire);
    }

    inline void begin()
    {
        g_Display.begin();
    }

    inline void set(char const* const szText)
    {
        g_Display.clearDisplay();

        g_Display.setTextSize(1);
        g_Display.setTextColor(SSD1306_WHITE);

        g_Display.setCursor(0, 0);
        g_Display.println(szText);

        g_Display.display();
    }
}  // namespace Display