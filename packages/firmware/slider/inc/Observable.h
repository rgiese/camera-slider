#pragma once

#include <atomic>
#include <functional>

//
// Note: Observables are intended to be created once and should not be deleted during the lifetime of the program
//       since we're not bothering to track them with shared_/weak_ptrs in our ObservableStore.
//

class ObservableStore;

class ObservableBase
{
protected:
    ObservableBase();

protected:
    friend class ObservableStore;

    virtual void deliver() = 0;
};

class ObservableStore
{
public:
    static ObservableStore& sharedInstance()
    {
        static ObservableStore globalStore;
        return globalStore;
    }

    void deliverAll()
    {
        for (ObservableBase* observable : m_Observables)
        {
            observable->deliver();
        }
    }

private:
    ObservableStore() = default;

    // Non-copyable
    ObservableStore(ObservableStore const&) = delete;
    ObservableStore& operator=(ObservableStore const&) = delete;

private:
    std::vector<ObservableBase*> m_Observables;

private:
    friend class ObservableBase;

    void registerObservable(ObservableBase* observable)
    {
        m_Observables.push_back(observable);
    }
};

template <typename T>
class Observable : public ObservableBase
{
public:
    Observable() = default;

    // Observed side
    void update(T const& value)
    {
        std::lock_guard<std::mutex> guard(m_Mutex);

        if (!(m_Value == value))  // Allow T to implement just '=='
        {
            m_Value = value;
            m_IsDirty = true;
        }
    }

    template <typename U = T>
    typename std::enable_if<std::is_integral<U>::value || std::is_enum<U>::value, U>::type get() const
    {
        return m_Value;
    }

    template <typename U = T>
    typename std::enable_if<!(std::is_integral<U>::value || std::is_enum<U>::value), U>::type get() const
    {
        std::lock_guard<std::mutex> guard(m_Mutex);

        T value = m_Value;

        return value;
    }

    operator T() const
    {
        return get();
    }

    // Observer side
    using Callback = std::function<void(T const&)>;

    void attach(Callback c)
    {
        std::lock_guard<std::mutex> guard(m_Mutex);
        m_Observers.push_back(c);
    }

    void attach_and_initialize(Callback c)
    {
        attach(c);

        T valueToDeliver;
        {
            std::lock_guard<std::mutex> guard(m_Mutex);
            valueToDeliver = m_Value;
        }

        c(valueToDeliver);
    }

private:
    // Non-copyable
    Observable<T>(Observable<T> const&) = delete;
    Observable<T>& operator=(Observable<T> const&) = delete;

private:
    std::vector<Callback> m_Observers;

    mutable std::mutex m_Mutex;

    T m_Value;
    bool m_IsDirty;

protected:
    void deliver() override
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
};