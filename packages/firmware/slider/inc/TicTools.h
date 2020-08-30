#pragma once

namespace TicTools
{
namespace Acceleration
{
inline uint32_t fromTicUnits(uint32_t const stepsPerSecPer100Sec)
{
    // Return steps per second per second
    return stepsPerSecPer100Sec / 100;
}

inline uint32_t toTicUnits(uint32_t const stepsPerSecPerSec)
{
    // Return steps per second per 100 seconds
    return stepsPerSecPerSec * 100;
}
}  // namespace Acceleration

namespace Speed
{
inline uint32_t fromTicUnits(uint32_t const stepsPer10KSec)
{
    // Return steps per second
    return stepsPer10KSec / 10000;
}

inline uint32_t toTicUnits(uint32_t const stepsPerSec)
{
    // Return steps per second per 10K seconds
    return stepsPerSec * 10000;
}

constexpr uint32_t c_MaxSafeHomingSpeed_StepsPerSec = 2000;

}  // namespace Speed
}  // namespace TicTools