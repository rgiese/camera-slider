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
    };

    MovementProgram() = default;

    static bool fromFlatbufferData(uint8_t const* const pData,
                                   size_t const cbData,
                                   _Out_ MovementProgram& movementProgram);

    void toFlatbufferData(flatbuffers::FlatBufferBuilder& flatbufferBuilder);

    Flatbuffers::Firmware::MovementProgramFlags Flags = Flatbuffers::Firmware::MovementProgramFlags::NONE;
    float Rate = 1;
    std::vector<Movement> Movements = {};

    // Debugging tools
    void dump(char const* const szPrefix) const;
};
