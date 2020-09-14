#pragma once

enum class RequestType
{
    UIButtonPressed,
    DesiredPosition,
    DesiredMaximumSpeed,
    DesiredMaximumAcceleration,
};

struct Request
{
    RequestType Type;

    union
    {
        struct
        {
        } UIButtonPressed;

        struct
        {
            int32_t value;
        } DesiredPosition;

        struct
        {
            uint32_t value;
        } DesiredMaximumSpeed;

        struct
        {
            uint32_t value;
        } DesiredMaximumAcceleration;
    };
};

class RequestQueue
{
public:
    RequestQueue() = default;

    void push(Request const& request);

    bool try_pop(__out Request& front);

private:
    std::mutex m_Mutex;
    std::queue<Request> m_Queue;

private:
    // Non-copyable
    RequestQueue(RequestQueue const&) = delete;
    RequestQueue& operator=(RequestQueue const&) = delete;
};

extern RequestQueue g_RequestQueue;