#include <stdint.h>
#include <stdio.h>

#include <cstdarg>
#include <cstring>
#include <ctime>
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>

// Test framework
#include "../../external/catch.hpp"

// Mocks
#include <Arduino.h>  // from mocks/platform

#include "mocks/adafruit_gfx.h"
#include "mocks/adafruit_ssd1306.h"

// Code-under-test
#include "inc/stdinc.h"
