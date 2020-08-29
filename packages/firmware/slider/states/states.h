#pragma once

// Main-level globals we depend on
extern TicI2C g_MotorController;

//
// Our state machine is articulated through transient objects derived from StateRoot.
// The lifetime of each state object is bound to the lifetime of the given state.
//
// The StateKeeper will transition state objects on loop ticks only
// so we can make sure only one state object is alive at a time.
//

// Forward declarations
class StateKeeper;

class InitializingMotorState;
class MotorHomingReverseState;
class MotorInitializedState;

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
    virtual void onUIButtonPressed(){};

protected:
    // Invoked via onLoop in StateKeeper
    friend class StateKeeper;
    virtual void onLoop(){};

private:
    // Non-copyable
    StateRoot(StateRoot const&) = delete;
    StateRoot& operator=(StateRoot const&) = delete;
};

template <typename _Derived>
class StateRootBase : public StateRoot
{
protected:
    // Invoked via StateCreatorFn in StateKeeper
    friend class StateKeeper;
    static StateRoot* createInstance()
    {
        return new _Derived();
    }
};

//
// State keeper
//

class StateKeeper
{
public:
    typedef StateRoot* (*StateCreatorFn)();

public:
    StateKeeper(){};

    // For use by main code
    StateRoot* operator->() const
    {
        return m_CurrentState.get();
    };

    void onLoop()
    {
        if (m_RequestedStateCreator)
        {
            // Delete current state object
            m_CurrentState.release();

            // Save off creator functor
            StateCreatorFn const requestedStateCreator = m_RequestedStateCreator;

            // Reset requested creator function in case it's already updated during the new state's constructor
            m_RequestedStateCreator = nullptr;

            // Invoke creator functor and store returned state object
            m_CurrentState.reset(requestedStateCreator());
        }

        if (m_CurrentState)
        {
            m_CurrentState->onLoop();
        }
    }

    // For use by state objects
    template <typename State>
    void RequestState()
    {
        m_RequestedStateCreator = &State::createInstance;
    }

private:
    std::unique_ptr<StateRoot> m_CurrentState;
    StateCreatorFn m_RequestedStateCreator;

private:
    // Non-copyable
    StateKeeper(StateKeeper const&) = delete;
    StateKeeper& operator=(StateKeeper const&) = delete;
};

StateKeeper g_State;

//
// Initializing motor
//
class InitializingMotorState : public StateRootBase<InitializingMotorState>
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
            g_State.RequestState<MotorHomingReverseState>();
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
class MotorHomingReverseState : public StateRootBase<MotorHomingReverseState>
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
            g_State.RequestState<InitializingMotorState>();
            return;
        }

        g_MotorController.goHomeReverse();
    }

    void onLoop() override
    {
        if (!g_MotorController.getHomingActive())
        {
            // Homing complete
            g_State.RequestState<MotorInitializedState>();
        }
    }
};

//
// Motor has been initalized
//
class MotorInitializedState : public StateRootBase<MotorInitializedState>
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
