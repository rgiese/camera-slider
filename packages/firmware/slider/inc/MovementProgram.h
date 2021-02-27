#pragma once

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
        };

        Movement() = default;

        Movement(Flatbuffers::Firmware::MovementType const type,
                 uint16_t const delayTime,
                 int32_t const desiredPosition,
                 uint32_t const desiredSpeed,
                 uint32_t const desiredAcceleration);

        Flatbuffers::Firmware::MovementType Type = Flatbuffers::Firmware::MovementType::Move;
        uint16_t DelayTime = 0;
        int32_t DesiredPosition = 0;
        uint32_t DesiredSpeed = 0;
        uint32_t DesiredAcceleration = 0;

        bool operator==(Movement const& other) const
        {
            if (Type != other.Type)
            {
                return false;
            }
            switch (Type)
            {
                case Flatbuffers::Firmware::MovementType::Delay:
                    return DelayTime == other.DelayTime;

                case Flatbuffers::Firmware::MovementType::Move:
                    return DesiredPosition == other.DesiredPosition && DesiredSpeed == other.DesiredSpeed &&
                           DesiredAcceleration == other.DesiredAcceleration;

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

    Flatbuffers::Firmware::MovementProgramFlags Flags = Flatbuffers::Firmware::MovementProgramFlags::NONE;
    uint16_t RatePercent = 100;
    std::vector<Movement> Movements = {};

    static constexpr uint16_t RatePercent_Minimum = 1;
    static constexpr uint16_t RatePercent_Maximum = 1000;

    //
    // Setters
    //

    void applyRateDelta(int16_t const RateDelta);

    //
    // Conversions
    //

    static bool fromFlatbufferData(uint8_t const* const pData,
                                   size_t const cbData,
                                   _Out_ MovementProgram& movementProgram);

    void toFlatbufferData(flatbuffers::FlatBufferBuilder& flatbufferBuilder) const;

    //
    // Comparison
    //

    bool operator==(MovementProgram const& other) const
    {
        return Flags == other.Flags && RatePercent == other.RatePercent && Movements == other.Movements;
    }

    //
    // Debugging tools
    //

    void dump(char const* const szPrefix) const;
};
