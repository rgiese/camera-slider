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
    g_Display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // Generate display voltage from 3.3V internally, 0x3C = I2C address
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