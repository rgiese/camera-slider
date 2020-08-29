#pragma once

// Main-level globals we depend on
extern TicI2C g_MotorController;

//
// Our state machine is articulated through transient objects derived from StateRoot.
// The lifetime of each state object is bound to the lifetime of the given state.
//

// Forward declarations
class StateRoot;

class InitializingMotorState;
class MotorHomingReverseState;
class MotorInitializedState;

// Global state pointer
std::unique_ptr<StateRoot> g_State;

template <typename State>
void transitionTo()
{
    g_State.reset(new State());
}

//
// State root
//

class StateRoot
{
public:
    StateRoot(){};
    virtual ~StateRoot(){};

    // Debugging tools (must implement)
    virtual char const* getName() = 0;

    // All handlers (optional to implement)
    virtual void onLoop(){};
    virtual void onUIButtonPressed(){};

private:
    // Non-copyable
    StateRoot(StateRoot const&) = delete;
    StateRoot& operator=(StateRoot const&) = delete;
};

//
// Initializing motor
//
class InitializingMotorState : public StateRoot
{
public:
    char const* getName() override
    {
        return "initializingMotor";
    }

    InitializingMotorState()
    {
        setDisplay("Initializing motor");
    }

    void onLoop() override
    {
        // Try to initialize motor
        if (tryInitializeMotor())
        {
            transitionTo<MotorHomingReverseState>();
            return;
        }
    }

private:
    bool tryInitializeMotor()
    {
        g_MotorController.exitSafeStart();
        g_MotorController.deenergize();

        if (g_MotorController.getOperationState() != TicOperationState::Deenergized)
        {
            return false;
        }

        g_MotorController.exitSafeStart();
        g_MotorController.energize();

        if (g_MotorController.getOperationState() != TicOperationState::Normal)
        {
            return false;
        }

        g_MotorController.exitSafeStart();
        g_MotorController.haltAndSetPosition(0);

        return true;
    }
};

//
// Homing motor to reverse end switch
//
class MotorHomingReverseState : public StateRoot
{
public:
    char const* getName() override
    {
        return "homingMotorReverse";
    }

    MotorHomingReverseState()
    {
        setDisplay("Homing motor");

        if (g_MotorController.getOperationState() != TicOperationState::Normal)
        {
            transitionTo<InitializingMotorState>();
            return;
        }

        g_MotorController.goHomeReverse();
    }

    void onLoop() override
    {
        if (!g_MotorController.getHomingActive())
        {
            // Homing complete
            transitionTo<MotorInitializedState>();
        }
    }
};

//
// Motor has been initalized
//
class MotorInitializedState : public StateRoot
{
public:
    char const* getName() override
    {
        return "motorInitialized";
    }

    MotorInitializedState()
    {
        setDisplay("Initialized");
    }
};
