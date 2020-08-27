#include <Particle.h>

#include "inc/stdinc.h"

//
// Particle configuration
//

PRODUCT_ID(11817);
PRODUCT_VERSION(1);  // Increment for each release


//
// Globals
//

pin_t constexpr c_UIButtonPin = D2;

Adafruit_SSD1306 g_Display = Adafruit_SSD1306(128, 32, &Wire);

//
// Declarations
//


//
// Setup
//

SYSTEM_THREAD(ENABLED);       // separate app thread from networking thread, c.f.
                              // https://docs.particle.io/reference/device-os/firmware/photon/#system-thread
SYSTEM_MODE(SEMI_AUTOMATIC);  // defer networking connection until explicit call, c.f.
                              // https://docs.particle.io/reference/device-os/firmware/photon/#semi-automatic-mode

void setup()
{
    // Configure debugging output
    Serial.begin();
    Serial.println("Slider started.");

    // Configure pins
    pinMode(c_UIButtonPin, INPUT_PULLUP);

    // Configure devices
    g_Display.begin();
    g_Display.display();  // Display logo

    delay(2000);

    g_Display.clearDisplay();
    g_Display.display();

    delay(2000);

    g_Display.invertDisplay(true);
    delay(2000);

    g_Display.invertDisplay(false);

    g_Display.setTextSize(1);
    g_Display.setTextColor(SSD1306_WHITE);
    g_Display.setCursor(0, 0);
    g_Display.println("Hello world!");
    g_Display.setCursor(0, 0);
    g_Display.display();

    // Request connection to cloud (not blocking)
    {
        Activity connectActivity("Connect");
        Particle.connect();
    }
}


//
// Loop
//

void loop()
{
    {
        Activity loopDelayActivity("LoopDelay");

        delay(1000);
    }
}
