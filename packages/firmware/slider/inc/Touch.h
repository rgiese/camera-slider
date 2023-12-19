#pragma once

class Touch
{
public:
    struct RawTouchPoint
    {
        float X;
        float Y;
        float Z;
    };

    static RawTouchPoint getRawTouchPoint();
};
