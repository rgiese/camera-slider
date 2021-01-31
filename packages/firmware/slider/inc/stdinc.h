#pragma once

#define PLATFORM_ID 12  // Particle Argon

#ifndef __STATIC_INLINE  // Fixes VSCode's C++ getting confused about the definition of uint32_t (long story...)
#define __STATIC_INLINE static inline
#endif

#include <Particle.h>

#include <stdio.h>
#include <stdarg.h>
#include <math.h>

#include <array>
#include <memory>
#include <mutex>
#include <queue>

// Core definitions
#include "inc/CoreDefs.h"
#include "inc/Activity.h"

// Shared dependencies
#include "generated/firmware_generated.h"

// Features
#include "inc/MovementProgram.h"
#include "inc/MovementProgramStore.h"
#include "inc/Display.h"
#include "inc/Encoder.h"
#include "bluetooth/Bluetooth.h"
#include "motor/MotorController.h"
#include "inc/Requests.h"
#include "states/states.h"
#include "inc/UI.h"
