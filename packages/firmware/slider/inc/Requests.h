#pragma once

enum class RequestType
{
    UIButtonPressed,
    DesiredPosition,
};

struct Request
{
    RequestType Type;

    union
    {
        // UIButtonPressed
        // - <nothing>

        // DesiredPosition
        int32_t DesiredPosition;
    } Data;
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