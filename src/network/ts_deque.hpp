#pragma once
#include <deque>
#include <mutex>
#include <condition_variable>

template <typename T>
class TSDeque
{
public:
    TSDeque() = default;
    ~TSDeque() { clear(); }

public:
    // Returns and maintains item at front of Queue
    const T& front()
    {
        std::scoped_lock<std::mutex> lock { deque_mutex };
        return deque.front();
    }

    // Returns and maintains item at back of Queue
    const T& back()
    {
        std::scoped_lock<std::mutex> lock { deque_mutex };
        return deque.back();
    }

    // Removes and returns item from front of Queue
    T pop_front()
    {
        std::scoped_lock<std::mutex> lock { deque_mutex };
        auto t = std::move(deque.front());
        deque.pop_front();
        return t;
    }

    // Removes and returns item from back of Queue
    T pop_back()
    {
        std::scoped_lock<std::mutex> lock { deque_mutex };
        auto t = std::move(deque.back());
        deque.pop_back();
        return t;
    }

    // Adds an item to back of Queue
    void push_back(const T& item)
    {
        {
            std::scoped_lock<std::mutex> lock { deque_mutex };
            deque.emplace_back(std::move(item));
        }
        notify_cv.notify_one();
    }

    // Adds an item to front of Queue
    void push_front(const T& item)
    {
        {
            std::scoped_lock<std::mutex> lock { deque_mutex };
            deque.emplace_front(std::move(item));
        }
        notify_cv.notify_one();
    }

    // Returns true if Queue has no items
    bool empty()
    {
        std::scoped_lock<std::mutex> lock { deque_mutex };
        return deque.empty();
    }

    // Returns number of items in Queue
    size_t count()
    {
        std::scoped_lock<std::mutex> lock { deque_mutex };
        return deque.size();
    }

    // Clears Queue
    void clear()
    {
        std::scoped_lock<std::mutex> lock { deque_mutex };
        deque.clear();
    }

    void wait_for_entries()
    {
        std::unique_lock<std::mutex> lock { deque_mutex };
        notify_cv.wait(lock, [this]()
            { return deque.size(); });
    }

protected:
    std::mutex deque_mutex;
    std::deque<T> deque;
    std::condition_variable notify_cv;
};