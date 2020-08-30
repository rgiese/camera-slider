#pragma once

#define SSD1306_BLACK 0    ///< Draw 'off' pixels
#define SSD1306_WHITE 1    ///< Draw 'on' pixels
#define SSD1306_INVERSE 2  ///< Invert pixels

class Adafruit_SSD1306 : public Adafruit_GFX
{
public:
    Adafruit_SSD1306(uint8_t w, uint8_t h, Wire *twi){};

public:
    void begin(uint8_t vcs = 0, uint8_t addr = 0){};

    void clearDisplay(){};
    void invertDisplay(bool){};
    void display(){};
};