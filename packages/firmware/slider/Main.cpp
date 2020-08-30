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

    // Delay for debugging (give serial monitor time to attach)
    delay(5000);

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

            dumpMotorControllerState();
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

void dumpMotorControllerState()
{
    Serial.println("Motor controller state:");

    // Controller state
    {
        TicOperationState const operationState = g_MotorController.getOperationState();
        Serial.print("  Operation state: ");

        switch (operationState)
        {
            case TicOperationState::Reset:
                Serial.println("reset");
                break;
            case TicOperationState::Deenergized:
                Serial.println("de-energized");
                break;
            case TicOperationState::SoftError:
                Serial.println("soft error");
                break;
            case TicOperationState::WaitingForErrLine:
                Serial.println("waiting for error line");
                break;
            case TicOperationState::StartingUp:
                Serial.println("starting up");
                break;
            case TicOperationState::Normal:
                Serial.println("normal");
                break;
            default:
                Serial.println("other");
                break;
        }
    }

    // Controller errors
    {
        uint16_t const errorStatus = g_MotorController.getErrorStatus();
        Serial.print("  Errors: ");

        if (errorStatus & (1 << static_cast<uint8_t>(TicError::IntentionallyDeenergized)))
        {
            Serial.print("intentionally-deenergized ");
        }

        if (errorStatus & (1 << static_cast<uint8_t>(TicError::MotorDriverError)))
        {
            Serial.print("motor-driver-error ");
        }

        if (errorStatus & (1 << static_cast<uint8_t>(TicError::LowVin)))
        {
            Serial.print("low-vin ");
        }
        if (errorStatus & (1 << static_cast<uint8_t>(TicError::KillSwitch)))
        {
            Serial.print("kill-switch ");
        }

        if (errorStatus & (1 << static_cast<uint8_t>(TicError::RequiredInputInvalid)))
        {
            Serial.print("required-input-invalid ");
        }

        if (errorStatus & (1 << static_cast<uint8_t>(TicError::SerialError)))
        {
            Serial.print("serial-error ");
        }

        if (errorStatus & (1 << static_cast<uint8_t>(TicError::CommandTimeout)))
        {
            Serial.print("command-timeout ");
        }

        if (errorStatus & (1 << static_cast<uint8_t>(TicError::SafeStartViolation)))
        {
            Serial.print("safe-start-violation ");
        }

        if (errorStatus & (1 << static_cast<uint8_t>(TicError::ErrLineHigh)))
        {
            Serial.print("error-line-high ");
        }

        Serial.println("");
    }

    // Motor configuration: physical
    {
        TicStepMode const stepMode = g_MotorController.getStepMode();
        uint16_t const currentLimit_mA = g_MotorController.getCurrentLimit();

        Serial.printlnf("  Step mode: %d", static_cast<uint8_t>(stepMode));
        Serial.printlnf("  Current limit: %u", currentLimit_mA);
    }

    // Motor configuration: logical
    {
        int32_t const maxSpeed = g_MotorController.getMaxSpeed();
        int32_t const maxAcceleration = g_MotorController.getMaxAccel();
        int32_t const maxDeceleration = g_MotorController.getMaxDecel();

        Serial.printlnf("  Max speed: %d steps/sec", TicTools::Speed::fromTicUnits(maxSpeed));
        Serial.printlnf("  Max acceleration: %d steps/sec^2, max deceleration: %d steps/sec^2",
                        TicTools::Acceleration::fromTicUnits(maxAcceleration),
                        TicTools::Acceleration::fromTicUnits(maxDeceleration));

        TicPlanningMode const planningMode = g_MotorController.getPlanningMode();

        Serial.print("  Planning mode: ");

        switch (planningMode)
        {
            case TicPlanningMode::Off:
                Serial.println("off");
                break;

            case TicPlanningMode::TargetPosition:
                Serial.println("target position");
                break;

            case TicPlanningMode::TargetVelocity:
                Serial.println("target velocity");
                break;

            default:
                Serial.println("unknown");
                break;
        }
    }

    // Controller state: voltage
    {
        int16_t const inputVoltage_mV = g_MotorController.getVinVoltage();

        Serial.printlnf("  Input voltage: %d mV", inputVoltage_mV);
    }

    // Motor state: planning
    {
        int32_t const targetPosition = g_MotorController.getTargetPosition();
        int32_t const targetVelocity = g_MotorController.getTargetVelocity();

        Serial.printlnf("  Target position: %d, target velocity: %d", targetPosition, targetVelocity);
    }

    // Motor state: execution
    {
        bool const isPositionUncertain = g_MotorController.getPositionUncertain();
        int32_t const currentPosition = g_MotorController.getCurrentPosition();
        int32_t const currentVelocity = g_MotorController.getCurrentVelocity();

        Serial.printlnf("  Current position (%s): %d, current velocity: %d",
                        isPositionUncertain ? "uncertain" : "certain",
                        currentPosition,
                        currentVelocity);

        bool const isForwardLimitActive = g_MotorController.getForwardLimitActive();
        bool const isReverseLimitActive = g_MotorController.getReverseLimitActive();
        bool const isHoming = g_MotorController.getHomingActive();

        Serial.printlnf("  Forward limit: %s, reverse limit: %s, homing: %s",
                        isForwardLimitActive ? "yes" : "no",
                        isReverseLimitActive ? "yes" : "no",
                        isHoming ? "yes" : "no");
    }

    Serial.println("");
}

void onUIButtonPressed()
{
    if (g_StateKeeper.CurrentState())
    {
        g_StateKeeper.CurrentState()->onUIButtonPressed();
    }
}
