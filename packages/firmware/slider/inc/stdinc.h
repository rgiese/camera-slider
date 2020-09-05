#pragma once

#define PLATFORM_ID 12  // Particle Argon

#include <Particle.h>

#include <stdio.h>
#include <stdarg.h>
#include <math.h>

#include <memory>
#include <mutex>
#include <queue>

// Core definitions
#include "inc/CoreDefs.h"
#include "inc/Activity.h"

// Dependencies
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Tic.h>

// Features
#include "inc/Display.h"
#include "inc/Bluetooth.h"
#include "inc/MotorController.h"
#include "inc/Requests.h"
#include "states/states.h"
