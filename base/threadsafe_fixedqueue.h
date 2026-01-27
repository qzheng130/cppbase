#pragma once
#include "circular_buffer.h"
#include <condition_variable>
#include <mutex>

// allow circular_buffer overflow/overwrite
template<typename T>
class threadsafe_fixedqueue
{
private:
    mutable std::mutex mutex_;
    std::condition_variable notEmpty_;
    circular_buffer<T> queue_;

public:
    explicit threadsafe_fixedqueue(int capacity)
        : queue_(capacity)
    {}

    threadsafe_fixedqueue()
        : queue_()
    {}

    void push(const T &x)
    {
        std::lock_guard<std::mutex> lock(mutex_);
        queue_.push(x);
        notEmpty_.notify_one();
    }

    void push(T &&x)
    {
        std::lock_guard<std::mutex> lock(mutex_);
        queue_.push(std::move(x));
        notEmpty_.notify_one();
    }

    void wait_and_pop(T &value)
    {
        std::unique_lock<std::mutex> lock(mutex_);
        notEmpty_.wait(lock, [this] { return !queue_.empty(); });
        value = std::move(queue_.front());
        queue_.pop();
    }

    std::shared_ptr<T> wait_and_pop()
    {
        std::unique_lock<std::mutex> lock(mutex_);
        notEmpty_.wait(lock, [this] { return !queue_.empty(); });
        std::shared_ptr<T> res = std::make_shared<T>(std::move(queue_.front()));
        queue_.pop();
        return res;
    }

    bool try_pop(T &value)
    {
        std::lock_guard<std::mutex> lk(mutex_);
        if (queue_.empty())
            return false;
        value = std::move(queue_.front());
        queue_.pop();
        return true;
    }

    std::shared_ptr<T> try_pop()
    {
        std::lock_guard<std::mutex> lk(mutex_);
        if (queue_.empty())
            return std::shared_ptr<T>();
        std::shared_ptr<T> res(std::make_shared<T>(std::move(queue_.front())));
        queue_.pop();
        return res;
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

    void set_capacity(size_t new_capacity)
    {
        std::lock_guard<std::mutex> lock(mutex_);
        queue_.set_capacity(new_capacity);
    }

    bool contains(const T &value) const
    {
        std::lock_guard<std::mutex> lock(mutex_);
        return queue_.contains(value);
    }
};

