#pragma once

#define PLATFORM_ID 12  // Particle Argon

#include <Particle.h>

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

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Globals
extern TicI2C g_MotorController;

// Features
#include "inc/Display.h"
#include "inc/Bluetooth.h"
#include "states/states.h"
