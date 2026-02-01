#pragma once
#include "join_threads.h"
#include "threadsafe_queue.h"
#include <atomic>
#include <future>
// #include <memory>
#include <functional>
#include <vector>

class thread_pool
{
public:
    thread_pool(unsigned thread_count = std::thread::hardware_concurrency())
        : thread_count_(thread_count)
        , done_(false)
        , joiner_(threads_)
    {
        try {
            for (unsigned i = 0; i < thread_count_; ++i) {
                threads_.push_back(std::thread(&thread_pool::worker_thread, this));
            }
        } catch (...) {
            done_ = true;
            throw;
        }
    }

    ~thread_pool() { stop(); }

    void stop() { done_ = true; }
    void restart() { done_ = false; }

    template<typename Func, typename... Args>
    auto submit(Func &&func, Args &&...args) -> std::future<decltype(func(args...))>
    {
        using result_type = decltype(func(args...));

        auto task = std::make_shared<std::packaged_task<result_type()>>(
            std::bind(std::forward<Func>(func), std::forward<Args>(args)...));

        std::future<result_type> res(task->get_future());

        work_queue_.push([task]() { (*task)(); });

        return res;
    }

private:
    void worker_thread()
    {
        while (!done_) {
            std::function<void(void)> task;
            if (work_queue_.try_pop(task)) {
                task();
            } else {
                std::this_thread::yield();
            }
        }
    }

    unsigned thread_count_;
    using threadsafe_queue_t = threadsafe_queue<std::function<void(void)>>;
    std::atomic<bool> done_;
    threadsafe_queue_t work_queue_;
    std::vector<std::thread> threads_;
    join_threads joiner_;
};
