#include "base.h"

Adafruit_GFX::Adafruit_GFX(int16_t w, int16_t h)
{
}

void Adafruit_GFX::startWrite(void){};
void Adafruit_GFX::writePixel(int16_t x, int16_t y, uint16_t color){};
void Adafruit_GFX::writeFillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color){};
void Adafruit_GFX::writeFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color){};
void Adafruit_GFX::writeFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color){};
void Adafruit_GFX::writeLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color){};
void Adafruit_GFX::endWrite(void){};
void Adafruit_GFX::setRotation(uint8_t r){};
void Adafruit_GFX::invertDisplay(bool i){};
void Adafruit_GFX::drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color){};
void Adafruit_GFX::drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color){};
void Adafruit_GFX::fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color){};
void Adafruit_GFX::fillScreen(uint16_t color){};
void Adafruit_GFX::drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color){};
void Adafruit_GFX::drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color){};

size_t Adafruit_GFX::write(uint8_t)
{
    return 0;
};
