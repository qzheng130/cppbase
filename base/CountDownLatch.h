#pragma once
#include <mutex>
#include <condition_variable>
#include "Noncopyable.h"
#include <cassert>

namespace cppbase
{
class CountDownLatch : Noncopyable
{
public:
    explicit CountDownLatch(int count)
    : count_(count) { };

    void wait()
    {
        std::unique_lock<std::mutex> lock(mutex_);
        condition_.wait(lock, [this] { return count_ == 0; });
        assert(count_ == 0);
    };

    void countDown()
    {
        std::lock_guard<std::mutex> lock(mutex_);
        --count_;
        if (count_ == 0)
        {
            condition_.notify_all();
        }
    };

    int getCount() const
    {
        std::lock_guard<std::mutex> lock(mutex_);
        return count_;
    };

private:
    mutable std::mutex mutex_;
    std::condition_variable condition_;
    int count_;
};
} // namespace cppbase
