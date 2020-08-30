#pragma once

//
// Our state machine is articulated through transient objects derived from StateRoot.
// The lifetime of each state object is bound to the lifetime of the given state.
//
// The StateKeeper will transition state objects on loop ticks only
// so we can make sure only one state object is operating at a time.
//

#include "AbstractState.h"
#include "StateKeeper.h"

extern StateKeeper g_StateKeeper;

// Forward declarations

// Terminal states
class UnrecoverableErrorState;

// Initialization states
class InitializingMotorState;
class MotorHomingReverseState;
class MotorInitializedState;

// Setup/admin states
class MotorFindingForwardRangeState;

#include "UnrecoverableErrorState.h"

#include "InitializingMotorState.h"
#include "MotorHomingReverseState.h"
#include "MotorInitializedState.h"

#include "MotorFindingForwardRangeState.h"
