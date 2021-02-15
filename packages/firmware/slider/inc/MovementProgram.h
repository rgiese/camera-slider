#pragma once

struct MovementProgram
{
    struct Movement
    {
        Movement() = default;

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
    };

    MovementProgram() = default;

    static bool fromFlatbufferData(uint8_t const* const pData,
                                   size_t const cbData,
                                   _Out_ MovementProgram& movementProgram);

    void toFlatbufferData(flatbuffers::FlatBufferBuilder& flatbufferBuilder) const;

    Flatbuffers::Firmware::MovementProgramFlags Flags = Flatbuffers::Firmware::MovementProgramFlags::NONE;
    uint16_t RatePercent = 100;
    std::vector<Movement> Movements = {};

    bool operator==(MovementProgram const& other) const
    {
        return Flags == other.Flags && RatePercent == other.RatePercent && Movements == other.Movements;
    }

    // Debugging tools
    void dump(char const* const szPrefix) const;
};
