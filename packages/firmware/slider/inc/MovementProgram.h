#pragma once

enum class MovementType
{
    Move,
    Delay,
};

struct MovementProgram
{
    struct Movement
    {
        Movement() = default;

        MovementType Type = MovementType::Move;
        uint16_t DelayTime = 0;
        int32_t DesiredPosition = 0;
        uint32_t DesiredSpeed = 0;
        uint32_t DesiredAcceleration = 0;
    };

    MovementProgram() = default;

    bool Repeat = false;
    float Rate = 1;
    std::vector<Movement> Movements = {};

    // Debugging tools
    void dump(char const* const szPrefix) const;
};
