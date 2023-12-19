#pragma once

enum class RequestType
{
    UIButtonPressed,
    DesiredPosition,
    DesiredMaximumSpeed,
    DesiredMaximumAcceleration,
    DesiredParameterDelta_Position,
    DesiredParameterDelta_MaximumSpeed,
    DesiredParameterDelta_MaximumAcceleration,
    StartMovementProgram,
    StopMovementProgram,
    UpdatedMovementProgram,
    CalibrateTouchStart,
    CalibrateTouchNext,
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
            uint32_t value;
        } DesiredMaximumSpeed;

        struct
        {
            uint32_t value;
        } DesiredMaximumAcceleration;

        struct
        {
            int32_t delta;
        } DesiredParameterDelta;

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

        struct
        {
        } CalibrateTouchStart;

        struct
        {
        } CalibrateTouchNext;
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