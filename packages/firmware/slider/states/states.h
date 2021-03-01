#pragma once

//
// Our state machine is articulated through transient objects derived from StateRoot.
// The lifetime of each state object is bound to the lifetime of the given state.
//
// The StateKeeper will transition state objects on loop ticks only
// so we can make sure only one state object is operating at a time.
//

#include "generated/sliderStates.h"

#include "AbstractState.h"
#include "StateKeeper.h"

extern StateKeeper g_StateKeeper;

#include "UnrecoverableErrorState.h"

#include "InitializingMotorState.h"
#include "HomingState.h"
#include "RunningMovementProgramState.h"
#include "TrackingDesiredPositionState.h"
#include "FindingForwardRangeState.h"
#include "CalibrateTouchScreenState.h"