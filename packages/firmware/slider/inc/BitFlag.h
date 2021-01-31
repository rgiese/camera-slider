#pragma once


template <typename T, typename Enable = void>
class BitFlag;

template <typename T>
class BitFlag<T, typename std::enable_if_t<std::is_enum<T>::value>>
{
public:
    typedef typename std::underlying_type_t<T> ValueType;

    constexpr BitFlag(const T& value)
        : m_Value(static_cast<ValueType>(value)){};

    template <typename... Args>
    constexpr BitFlag(const T& value, Args... args)
        : m_Value(static_cast<ValueType>(value) | BitFlag(args...)){};

    constexpr operator ValueType() const
    {
        return m_Value;
    }

private:
    ValueType const m_Value;
};
