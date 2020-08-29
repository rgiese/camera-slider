#include "inc/stdinc.h"
#include "inc/Button.h"
#include <Tic.h>

#include "states/states.h"

//
// Particle configuration
//

PRODUCT_ID(11817);
PRODUCT_VERSION(1);  // Increment for each release

//
// Declarations
//

void onUIButtonPressed();

//
// Globals
//

Adafruit_SSD1306 g_Display = Adafruit_SSD1306(128, 32, &Wire);
TicI2C g_MotorController(14 /* default I2C address */);
Button g_UIButton(D2, &onUIButtonPressed);


//
// Setup
//

SYSTEM_THREAD(ENABLED);       // separate app thread from networking thread, c.f.
                              // https://docs.particle.io/reference/device-os/firmware/photon/#system-thread
SYSTEM_MODE(SEMI_AUTOMATIC);  // defer networking connection until explicit call, c.f.
                              // https://docs.particle.io/reference/device-os/firmware/photon/#semi-automatic-mode

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

    delay(5000);

    // Configure motor controller
    delay(100);  // Give motor controller time to start

    g_MotorController.setProduct(TicProduct::T500);

    // Request connection to cloud (not blocking)
    {
        Activity connectActivity("Connect");
        Particle.connect();
    }

    // Configure state
    g_State.reset(new InitializingMotorState());
}


//
// Loop
//

void loop()
{
    static uint16_t s_LoopCounter = 0;

    // Reset watchdog on motor controller
    g_MotorController.resetCommandTimeout();

    // Deliver interrupt-sourced events
    g_UIButton.onLoop();

    // Advance state machine
    g_State->onLoop();

    // Print debug stats
    if (s_LoopCounter % 200 == 0)
    {
        Serial.printlnf("-- Current state: %s", g_State->getName());

        dumpMotorControllerState();
    }

    // Loop around (needs to be under 1000ms so the motor controller watchdog doesn't trigger)
    delay(10);
    ++s_LoopCounter;
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
        int32_t const startingSpeed = g_MotorController.getStartingSpeed();
        int32_t const maxAcceleration = g_MotorController.getMaxAccel();
        int32_t const maxDeceleration = g_MotorController.getMaxDecel();

        Serial.printlnf(
            "  Max speed: %d microsteps/10Ksec, starting speed: %d microsteps/10Ksec", maxSpeed, startingSpeed);
        Serial.printlnf("  Max acceleration: %d microsteps/sec*100sec, max deceleration: %d microsteps/sec*100sec",
                        maxAcceleration,
                        maxDeceleration);

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
    g_State->onUIButtonPressed();
}
