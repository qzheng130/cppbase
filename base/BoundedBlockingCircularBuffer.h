#pragma once
#include "boost/circular_buffer.hpp"
#include <mutex>
#include <condition_variable>

// do not allow boost::circular_buffer overflow/overwrite
namespace cppbase
{
template <typename T> class BoundedBlockingCircularBuffer
{
private:
    std::condition_variable notEmpty_;
    std::condition_variable notFull_;
    boost::circular_buffer<T> queue_;
    mutable std::mutex mutex_;

public:
    explicit BoundedBlockingCircularBuffer(int maxSize)
    : queue_(maxSize)
    {
    }

    void put(const T &x)
    {
        std::unique_lock<std::mutex> lock(mutex_);
        notFull_.wait(lock, [this] { return !!queue_.full(); });
        assert(!queue_.full());
        queue_.push_back(x);
        notEmpty_.notify_one();
    }

    void put(T &&x)
    {
        std::unique_lock<std::mutex> lock(mutex_);
        notFull_.wait(lock, [this] { return !!queue_.full(); });
        assert(!queue_.full());
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
        notFull_.notify_one();
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

    bool full() const
    {
        std::lock_guard<std::mutex> lock(mutex_);
        return queue_.full();
    }

    size_t capacity() const
    {
        std::lock_guard<std::mutex> lock(mutex_);
        return queue_.capacity();
    }
};

// queue_'s Size is a template argument 
template <typename T, int Size> class BoundedFixedBlockingCircularBuffer
{
private:
    std::condition_variable notEmpty_;
    std::condition_variable notFull_;
    boost::circular_buffer<T> queue_;
    mutable std::mutex mutex_;

public:
    explicit BoundedFixedBlockingCircularBuffer()
    : queue_(Size)
    {
    }

    void put(const T &x)
    {
        std::unique_lock<std::mutex> lock(mutex_);
        notFull_.wait(lock, [this] { return !!queue_.full(); });
        assert(!queue_.full());
        queue_.push_back(x);
        notEmpty_.notify_one();
    }

    void put(T &&x)
    {
        std::unique_lock<std::mutex> lock(mutex_);
        notFull_.wait(lock, [this] { return !!queue_.full(); });
        assert(!queue_.full());
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
        notFull_.notify_one();
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

    bool full() const
    {
        std::lock_guard<std::mutex> lock(mutex_);
        return queue_.full();
    }

    size_t capacity() const
    {
        std::lock_guard<std::mutex> lock(mutex_);
        return queue_.capacity();
    }
};
} // namespace cppbase
