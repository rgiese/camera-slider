#pragma once

#include "MovementProgramOwner.h"

enum class RequestType
{
    UIButtonPressed,
    DesiredPosition,
    DesiredMaximumSpeed,
    DesiredMaximumAcceleration,
    DesiredMovementProgram,
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
    };

    // Non-union-able types
    struct
    {
        std::shared_ptr<MovementProgramOwner> MovementProgram;
    } DesiredMovementProgram;
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