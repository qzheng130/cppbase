#pragma once
#include "boost/circular_buffer.hpp"
#include <mutex>
#include <condition_variable>

// allow boost::circular_buffer overflow/overwrite
namespace cppbase
{
template <typename T> class BlockingCircularBuffer
{
private:
    mutable std::mutex mutex_;
    std::condition_variable notEmpty_;
    boost::circular_buffer<T> queue_;

public:
    explicit BoundedBlockingCircularBuffer(int maxSize)
    : queue_(maxSize)
    {
    }

    void put(const T &x)
    {
        std::lock_guard<std::mutex> lock(mutex_);
        queue_.push_back(x);
        notEmpty_.notify_one();
    }

    void put(T &&x)
    {
        std::lock_guard<std::mutex> lock(mutex_);
        queue_.push_back(std::move(x));
        notEmpty_.notify_one();
    }

    T take()
    {
        std::unique_lock<std::mutex> lock(mutex_);
        queue_.wait(lock, [this] { return !queue_.empty(); });
        assert(!queue_.empty());
        T front(std::move(queue_.front()));
        queue_.pop_front();
        return front;
    }

    bool empty() const
    {
        std::lock_guard<std::mutex> lock(mutex_);
        return queue_.empty();
    }

    size_t size() const
    {
        std::lock_guard<std::mutex> lock(mutex_);
        return queue_.size();
    }

    size_t capacity() const
    {
        std::lock_guard<std::mutex> lock(mutex_);
        return queue_.capacity();
    }
};

// queue_'s Size is a template argument 
template <typename T, int Size> class FixedBlockingCircularBuffer
{
private:
    mutable std::mutex mutex_;
    std::condition_variable notEmpty_;
    boost::circular_buffer<T> queue_;

public:
    explicit BoundedFixedBlockingCircularBuffer()
    : queue_(Size)
    {
    }

    void put(const T &x)
    {
        std::lock_guard<std::mutex> lock(mutex_);
        queue_.push_back(x);
        notEmpty_.notify_one();
    }

    void put(T &&x)
    {
        std::lock_guard<std::mutex> lock(mutex_);
        queue_.push_back(std::move(x));
        notEmpty_.notify_one();
    }

    T take()
    {
        std::unique_lock<std::mutex> lock(mutex_);
        notEmpty_.wait(lock, [this] { return !queue_.empty(); });
        assert(!queue_.empty());
        T front(std::move(queue_.front()));
        queue_.pop_front();
        return front;
    }

    bool empty() const
    {
        std::lock_guard<std::mutex> lock(mutex_);
        return queue_.empty();
    }

    size_t size() const
    {
        std::lock_guard<std::mutex> lock(mutex_);
        return queue_.size();
    }

    size_t capacity() const
    {
        std::lock_guard<std::mutex> lock(mutex_);
        return queue_.capacity();
    }
};
} // namespace cppbase
