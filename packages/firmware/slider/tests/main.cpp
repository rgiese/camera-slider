#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this here
#include "base.h"

// Instantiate mock globals
MockEEPROM EEPROM;
MockParticle Particle;
MockSerial Serial;
MockSystem System;
MockTime Time;
MockWiFi WiFi;

TwoWire Wire;

// Bogus test case
TEST_CASE("Tests execute", "[basics]")
{
    REQUIRE(1 == 1);
}
