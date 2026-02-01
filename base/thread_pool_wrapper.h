#pragma once
#include "join_threads.h"
#include "threadsafe_queue.h"
#include <atomic>
#include <future>
#include <memory>
#include <vector>

class function_wrapper
{
private:
    struct impl_base {
        virtual void call()=0;
        virtual ~impl_base() {}
    };

    // 使用独占指针引用到一个包含function对象类型impl
    std::unique_ptr<impl_base> impl;

    template<typename F>
    struct impl_type: impl_base
    {
        F f_;
        impl_type(F &&f)
            : f_(std::move(f))
        {}
        void call() { f_(); }
    };

public:
    // 使用function对象构造
    template<typename F>
    function_wrapper(F&& f):
        impl(new impl_type<F>(std::move(f)))
    {}

    void operator()() { impl->call(); }

    function_wrapper() = default;

    // 任何callable对象都可以移动构造为function_wrapper
    function_wrapper(function_wrapper&& other):
        impl(std::move(other.impl))
    {}

    // 移动赋值
    function_wrapper& operator=(function_wrapper&& other)
    {
        impl=std::move(other.impl);
        return *this;
    }

    // 不能拷贝构造和拷贝赋值
    function_wrapper(const function_wrapper&)=delete;
    function_wrapper(function_wrapper&)=delete;
    function_wrapper& operator=(const function_wrapper&)=delete;
};

class thread_pool
{
public:
    thread_pool()
        : done_(false)
        , joiner(threads_)
    {
        unsigned const thread_count = std::thread::hardware_concurrency();
        try {
            for (unsigned i = 0; i < thread_count; ++i) {
                threads_.push_back(std::thread(&thread_pool::worker_thread, this));
            }
        } catch (...) {
            done_ = true;
            throw;
        }
    }

    ~thread_pool() { done_ = true; }

    template<typename FunctionType>
    std::future<typename std::result_of<FunctionType()>::type>
    submit(FunctionType f)
    {
        // 函数对象没有参数
        // result_type函数对象调用返回值类型
        using result_type = typename std::result_of<FunctionType()>::type;

        std::packaged_task<result_type()> task(std::move(f));
        // 获取future
        std::future<result_type> res(task.get_future());
        work_queue_.push(std::move(task));
        return res;
    }

private:
    void worker_thread()
    {
        while (!done_) {
            function_wrapper task;
            if (work_queue_.try_pop(task)) {
                task();
            } else {
                std::this_thread::yield();
            }
        }
    }

    std::atomic<bool> done_;
    // 任务队列保存只能移动的function_wrapper
    threadsafe_queue<function_wrapper> work_queue_;
    std::vector<std::thread> threads_;
    join_threads joiner;
};
