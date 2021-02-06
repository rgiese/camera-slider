#pragma once

#include <atomic>
#include <functional>

template <typename T>
class Observable
{
public:
    Observable() = default;

    // Observed side
    void update(T const& value)
    {
        std::lock_guard<std::mutex> guard(m_Mutex);

        if (!(m_Value == value)) // Allow T to implement just '=='
        {
            m_Value = value;
            m_IsDirty = true;
        }
    }

    operator T const &() const
    {
        return m_Value;
    }

    T const& get() const  // use when auto-casting is ambiguous
    {
        return m_Value;
    }

    // Observer side
    using Callback = std::function<void(T const&)>;

    void attach(Callback c)
    {
        std::lock_guard<std::mutex> guard(m_Mutex);
        m_Observers.push_back(c);
    }

    void deliver()
    {
        T valueToDeliver;
        {
            std::lock_guard<std::mutex> guard(m_Mutex);

            if (!m_IsDirty)
            {
                return;
            }

            valueToDeliver = m_Value;
            m_IsDirty = false;
        }


        for (auto& observer : m_Observers)
        {
            observer(valueToDeliver);
        }
    }

private:
    // Non-copyable
    Observable<T>(Observable<T> const&) = delete;
    Observable<T>& operator=(Observable<T> const&) = delete;

private:
    std::vector<Callback> m_Observers;

    std::mutex m_Mutex;

    T m_Value;
    bool m_IsDirty;
};