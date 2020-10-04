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
    g_Display.set("Starting...");

    // Configure motor controller
    g_MotorController.begin();

    // Configure BLE
    g_Bluetooth.begin();

    // Request connection to cloud (not blocking)
    {
        // Activity connectActivity("Connect");
        // Particle.connect();
        WiFi.off();
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

    {
        Activity mainLoopActivity("mainThreadLoop", 10);

        {
            Activity mainLoopSectionActivity("bluetoothState", 10);
            g_Bluetooth.statusService().onMainThreadLoop();
        }
    }

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
        {
            Activity mainLoopActivity("stateThreadLoop", 10);

            {
                Activity mainLoopSectionActivity("motorController", 10);
                g_MotorController.onLoop();
            }

            {
                Activity mainLoopSectionActivity("bluetoothState", 10);
                g_Bluetooth.statusService().onStateMachineThreadLoop();
            }

            // Deliver interrupt-sourced events (creates Requests)
            {
                Activity mainLoopSectionActivity("uiButton", 10);
                g_UIButton.onLoop();
            }

            // Advance state machine (moves state, delivers Requests)
            {
                Activity mainLoopSectionActivity("stateKeeper", 10);
                g_StateKeeper.onLoop();
            }

            // Print debug stats
            if (loopCounter % 200 == 0)
            {
                Activity mainLoopSectionActivity("debugStats", 10);

                if (Serial.isConnected())
                {
                    g_MotorController.dumpState();
                }
            }
        }

        // Loop delay (needs to be under 1000ms so the motor controller watchdog doesn't trigger)
        os_thread_delay_until(&previousWakeTime, 25 /* msec */);
    }
}


//
// Helpers
//

void onUIButtonPressed()
{
    g_RequestQueue.push({Type : RequestType::UIButtonPressed});
}
