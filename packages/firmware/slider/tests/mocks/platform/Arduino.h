#pragma once

typedef uint8_t byte;
typedef uint16_t pin_t;

// These headers are in the current directory which is an include root since some dependencies include them as e.g.
// <Wire.h>
#include "Print.h"
#include "Stream.h"
#include "Wire.h"

// These headers are in a private subdirectory so they don't conflict with CRT headers (e.g. <time.h>)
#include "features/eeprom.h"
#include "features/io.h"
#include "features/locks.h"
#include "features/particle.h"
#include "features/pinmap_defines.h"
#include "features/serial.h"
#include "features/string.h"
#include "features/system.h"
#include "features/time.h"
#include "features/wifi.h"
