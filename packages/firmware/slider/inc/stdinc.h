#pragma once

#ifndef IS_TEST_BUILD
#include <Particle.h>
#endif

#include <stdio.h>
#include <stdarg.h>
#include <math.h>

#include <memory>

// Dependencies
#include <Tic.h>
#include "TicTools.h"

#ifndef IS_TEST_BUILD
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#endif

// Core definitions
#include "inc/CoreDefs.h"
#include "inc/Activity.h"

// Forward declarations
extern void setDisplay(char const* const szText);

// Globals
extern TicI2C g_MotorController;

// Features
#include "states/states.h"
