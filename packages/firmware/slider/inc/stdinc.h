#pragma once

#ifndef IS_TEST_BUILD
#include <Particle.h>
#endif

#include <stdio.h>
#include <stdarg.h>
#include <math.h>

#include <memory>

// Core definitions
#include "inc/CoreDefs.h"
#include "inc/Activity.h"

// Dependencies
#include <Tic.h>
#include "TicTools.h"

#ifndef IS_TEST_BUILD
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#endif

// Globals
extern TicI2C g_MotorController;

// Features
#include "inc/Display.h"
#include "states/states.h"
