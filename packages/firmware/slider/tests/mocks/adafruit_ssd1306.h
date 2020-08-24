#pragma once

class Adafruit_SSD1306
{
public:
    Adafruit_SSD1306(uint8_t w, uint8_t h, MockWire *twi){};

public:
    void begin(uint8_t vcs, uint8_t addr){};
    void display(){};
};