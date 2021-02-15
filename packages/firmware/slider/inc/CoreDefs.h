#pragma once

#define _In_
#define _Out_

#define RETURN_IF_FALSE(x) \
    if (!(x))              \
    {                      \
        return false;      \
    }

template <typename T, std::size_t N>
constexpr std::size_t countof(T const (&)[N]) noexcept
{
    return N;
}

template <typename T, std::size_t N>
constexpr std::size_t static_strlen(T const (&)[N]) noexcept
{
    return N - 1;
}


template <class T>
constexpr T clamp(T const value, T const low, T const high)
{
    return (value < low) ? low : (high < value) ? high : value;
}

template <class T, typename U = ssize_t>
constexpr T clamp_delta(T const value, U const delta, T const low, T const high)
{
    U const computedValue = value + delta;

    return (computedValue < low) ? low : (high < computedValue) ? high : static_cast<T>(computedValue);
}

static constexpr uint16_t __bswap_16(uint16_t const value)
{
    return (value >> 8) | (value << 8);
}
