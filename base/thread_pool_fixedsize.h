#pragma once
#include "join_threads.h"
#include "threadsafe_fixedqueue.h"
#include <atomic>
#include <functional>
#include <future>
#include <string>
#include <vector>

class thread_pool_fixedsize
{
public:
    typedef std::function<void()> task_t;

    explicit thread_pool_fixedsize(int queue_capacity,
                                   unsigned num_threads = std::thread::hardware_concurrency())
        : num_threads_(num_threads)
        , done_(false)
        , work_queue_(queue_capacity)
        , joiner_(threads_)
    {
        // unsigned const thread_count = std::thread::hardware_concurrency();

        try {
            for (int i = 0; i < num_threads_; ++i) {
                threads_.push_back(std::thread(&thread_pool_fixedsize::worker_thread, this));
            }
        } catch (...) {
            done_ = true;
            throw;
        }
    }

    ~thread_pool_fixedsize() { done_ = true; }

    void set_max_queue_size(int max_size) { work_queue_.set_capacity(max_size); }

    void stop() { done_ = true; };
    int queue_size() const { return work_queue_.size(); }

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
            task_t task;
            if (work_queue_.try_pop(task)) {
                task();
            } else {
                std::this_thread::yield();
            }
        }
    }

    unsigned num_threads_;
    std::atomic<bool> done_;

    using threadsafe_queue_t = threadsafe_fixedqueue<task_t>;
    threadsafe_queue_t work_queue_;
    std::vector<std::thread> threads_;
    join_threads joiner_;
};
