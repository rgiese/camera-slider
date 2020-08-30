#include "base.h"

Adafruit_SSD1306::Adafruit_SSD1306(
    uint8_t w, uint8_t h, TwoWire *twi, int8_t rst_pin, uint32_t clkDuring, uint32_t clkAfter)
    : Adafruit_GFX(w, h)
{
}

Adafruit_SSD1306::~Adafruit_SSD1306()
{
}

bool Adafruit_SSD1306::begin(uint8_t switchvcc, uint8_t i2caddr, bool reset, bool periphBegin)
{
    return true;
};

void Adafruit_SSD1306::clearDisplay(){};
void Adafruit_SSD1306::invertDisplay(bool){};
void Adafruit_SSD1306::display(){};
void Adafruit_SSD1306::drawPixel(int16_t x, int16_t y, uint16_t color){};
void Adafruit_SSD1306::drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color){};
void Adafruit_SSD1306::drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color){};
