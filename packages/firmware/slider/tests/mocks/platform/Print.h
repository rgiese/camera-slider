#pragma once

class Print
{
public:
    size_t write(uint8_t)
    {
        return 1;
    };

    void print(const char*){};
    void printf(const char*, ...){};
    void println(const char*){};
    void printlnf(const char*, ...){};
};