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
        enum class Parameter
        {
            DelayTime,
            DesiredPosition,
            DesiredSpeed,
            DesiredAcceleration,
            DesiredDeceleration,
        };

        Movement() = default;

        Movement(MovementType const type,
                 uint16_t const delayTime,
                 int32_t const desiredPosition,
                 uint32_t const desiredSpeed,
                 uint32_t const desiredAcceleration,
                 uint32_t const desiredDeceleration);

        MovementType Type = MovementType::Move;
        uint16_t DelayTime = 0;
        int32_t DesiredPosition = 0;
        uint32_t DesiredSpeed = 0;
        uint32_t DesiredAcceleration = 0;
        uint32_t DesiredDeceleration = 0;

        bool operator==(Movement const& other) const
        {
            if (Type != other.Type)
            {
                return false;
            }
            switch (Type)
            {
                case MovementType::Delay:
                    return DelayTime == other.DelayTime;

                case MovementType::Move:
                    return DesiredPosition == other.DesiredPosition && DesiredSpeed == other.DesiredSpeed &&
                           DesiredAcceleration == other.DesiredAcceleration &&
                           DesiredDeceleration == other.DesiredDeceleration;

                default:
                    return true;
            }
        }

        void applyDelta(Parameter const parameter, int32_t delta);
    };

    MovementProgram() = default;

    //
    // Data
    //

    bool Repeat = true;
    uint16_t RatePercent = 100;
    std::vector<Movement> Movements = {};

    static constexpr uint16_t RatePercent_Minimum = 1;
    static constexpr uint16_t RatePercent_Maximum = 1000;

    //
    // Setters
    //

    void applyRateDelta(int16_t const RateDelta);

    //
    // Execution
    //

    void requestMoveToMovement(size_t const idxMovement) const;

    //
    // Comparison
    //

    bool operator==(MovementProgram const& other) const
    {
        return Repeat == other.Repeat && RatePercent == other.RatePercent && Movements == other.Movements;
    }

    //
    // Debugging tools
    //

    void dump(char const* const szPrefix) const;
};
