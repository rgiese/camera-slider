#pragma once

#ifndef IS_TEST_BUILD
#include <Particle.h>
#endif

#include <math.h>

#include <memory>

// Dependencies
#ifndef IS_TEST_BUILD
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#endif

// Core definitions
#include "inc/CoreDefs.h"
#include "inc/Activity.h"

// Globals
extern void setDisplay(char const* const szText);
extern void dumpMotorControllerState();
