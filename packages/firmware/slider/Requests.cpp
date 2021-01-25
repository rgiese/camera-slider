#include "inc/stdinc.h"

RequestQueue g_RequestQueue;

void RequestQueue::push(Request const& request)
{
    std::lock_guard<std::mutex> guard(m_Mutex);

    m_Queue.push(request);
}

bool RequestQueue::try_pop(_Out_ Request& front)
{
    std::lock_guard<std::mutex> guard(m_Mutex);

    if (!m_Queue.empty())
    {
        front = m_Queue.front();
        m_Queue.pop();
        return true;
    }

    return false;
}
