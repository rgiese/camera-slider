#pragma once

class Activity
{
public:
    Activity(char const* const szName, unsigned long const minimumTimeToReport_msec = 0)
        : m_szName(szName), m_StartTime_msec(millis()), m_MinimumTimeToReport_msec(minimumTimeToReport_msec)
    {
        if (!m_MinimumTimeToReport_msec)
        {
            WITH_LOCK(Serial)
            {
                Serial.printlnf(">> %s (SSID: %s)", m_szName, getSSID());
            }
        }
    }

    ~Activity()
    {
        unsigned long const endTime_msec = millis();
        unsigned long const duration_msec = endTime_msec - m_StartTime_msec;

        if (!m_MinimumTimeToReport_msec || duration_msec > m_MinimumTimeToReport_msec)
        {
            WITH_LOCK(Serial)
            {
                Serial.printlnf("%s %s (%lu msec) (SSID: %s)",
                                m_MinimumTimeToReport_msec ? "<>" : "<<",
                                m_szName,
                                duration_msec,
                                getSSID());
            }
        }
    }

private:
    char const* const m_szName;
    unsigned long const m_StartTime_msec;
    unsigned long const m_MinimumTimeToReport_msec;

    char const* getSSID() const
    {
        return Particle.connected() ? WiFi.SSID() : "<not connected>";
    }

private:
    // Non-copyable
    Activity(Activity const&) = delete;
    Activity& operator=(Activity const&) = delete;
};