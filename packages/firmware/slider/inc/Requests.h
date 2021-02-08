#pragma once

enum class RequestType
{
    UIButtonPressed,
    DesiredPosition,
    DesiredPositionDelta,
    DesiredMaximumSpeed,
    DesiredMaximumSpeedDelta,
    DesiredMaximumAcceleration,
    DesiredMaximumAccelerationDelta,
    StartMovementProgram,
    StopMovementProgram,
    UpdatedMovementProgram,
};

struct Request
{
    RequestType Type;

    // Union-able types
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
            int32_t delta;
        } DesiredPositionDelta;

        struct
        {
            uint32_t value;
        } DesiredMaximumSpeed;

        struct
        {
            int32_t delta;
        } DesiredMaximumSpeedDelta;

        struct
        {
            uint32_t value;
        } DesiredMaximumAcceleration;

        struct
        {
            int32_t delta;
        } DesiredMaximumAccelerationDelta;

        struct
        {
        } UpdatedMovementProgram;

        struct
        {
            uint32_t atStep;
        } StartMovementProgram;

        struct
        {
        } StopMovementProgram;
    };
};

class RequestQueue
{
public:
    RequestQueue() = default;

    void push(Request const& request);

    bool try_pop(_Out_ Request& front);

private:
    std::mutex m_Mutex;
    std::queue<Request> m_Queue;

private:
    // Non-copyable
    RequestQueue(RequestQueue const&) = delete;
    RequestQueue& operator=(RequestQueue const&) = delete;
};

extern RequestQueue g_RequestQueue;