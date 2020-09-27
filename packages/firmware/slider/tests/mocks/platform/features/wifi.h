#pragma once

class MockWiFi
{
public:
    MockWiFi()
    {
    }

public:
    void off()
    {
    }

    char const* SSID() const
    {
        return "MockWiFi";
    }
};

extern MockWiFi WiFi;