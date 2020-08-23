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

pin_t constexpr c_LedPin = D7;

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

    {
        Activity testActivity("StartupTestDelay");
        delay(10 * 1000);
    }

    pinMode(c_LedPin, OUTPUT);

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
