#include "inc/stdinc.h"
#include "inc/Button.h"

//
// Particle configuration
//

PRODUCT_ID(11817);
PRODUCT_VERSION(1);  // Increment for each release

//
// Declarations
//

void onUIButtonPressed();
void stateMachineThreadFn(void*);

//
// Globals
//

Adafruit_SSD1306 g_Display = Adafruit_SSD1306(128, 32, &Wire);
TicI2C g_MotorController(14 /* default I2C address */);
Button g_UIButton(D2, &onUIButtonPressed);


//
// Setup
//

SYSTEM_THREAD(ENABLED);  // separate app thread from networking thread, c.f.
                         // https://docs.particle.io/reference/device-os/firmware/photon/#system-thread
SYSTEM_MODE(MANUAL);     // defer networking connection until explicit call, c.f.
                         // https://docs.particle.io/reference/device-os/firmware/argon/#system-modes

void setup()
{
    //
    // Configure system
    //

    // Configure debugging output
    Serial.begin();
    Serial.println("Slider started.");

    // Configure display
    g_Display.begin();
    setDisplay("Starting...");

    // Configure motor controller
    delay(100);  // Give motor controller time to start

    g_MotorController.setProduct(TicProduct::T500);

    // Request connection to cloud (not blocking)
    {
        Activity connectActivity("Connect");
        Particle.connect();
    }

    // Kick off state machine
    {
        os_thread_t threadHandle;
        os_thread_create(&threadHandle,
                         "stateMachine",
                         7,  // Minimum priority required so we don't get preempted by WiFi management thread
                         &stateMachineThreadFn,
                         nullptr,
                         OS_THREAD_STACK_SIZE_DEFAULT);
    }
}


//
// Loop
//

void loop()
{
    delay(100);

    if (Particle.connected())
    {
        Particle.process();
    }
}


//
// State machine thread
//

void stateMachineThreadFn(void*)
{
    // Configure state
    g_StateKeeper.RequestState(new InitializingMotorState());

    // Run state machine
    system_tick_t previousWakeTime = 0;

    for (uint16_t loopCounter = 0; /* forever */; ++loopCounter)
    {
        // Reset watchdog on motor controller
        g_MotorController.resetCommandTimeout();

        // Advance state machine
        g_StateKeeper.onLoop();

        // Deliver interrupt-sourced events
        g_UIButton.onLoop();

        // Print debug stats
        if (loopCounter % 200 == 0)
        {
            if (g_StateKeeper.CurrentState())
            {
                Serial.printlnf("-- Current state: %s", g_StateKeeper.CurrentState()->getName());
            }
            else
            {
                Serial.println("-- Current state: <stateless>");
            }

            TicTools::Debug::dumpMotorControllerState(g_MotorController);
        }

        // Loop delay (needs to be under 1000ms so the motor controller watchdog doesn't trigger)
        os_thread_delay_until(&previousWakeTime, 100 /* msec */);
    }
}


//
// Helpers
//

void setDisplay(char const* const szText)
{
    g_Display.clearDisplay();

    g_Display.setTextSize(1);
    g_Display.setTextColor(SSD1306_WHITE);

    g_Display.setCursor(0, 0);
    g_Display.println(szText);

    g_Display.display();
}

void onUIButtonPressed()
{
    if (g_StateKeeper.CurrentState())
    {
        g_StateKeeper.CurrentState()->onUIButtonPressed();
    }
}
