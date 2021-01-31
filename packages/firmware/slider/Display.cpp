#include "inc/stdinc.h"

Display g_Display;

void Display::begin()
{
}

void Display::set(char const* const szText)
{
    WITH_LOCK(Serial)
    {
        Serial.print("-- Display update: ");
        Serial.println(szText);
    }
}
