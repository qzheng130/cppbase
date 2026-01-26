#include "thread_pool.h"

thread_pool::thread_pool()
: done(false)
, joiner(threads)
{
    unsigned const thread_count = std::thread::hardware_concurrency();
    try
    {
        for (unsigned i = 0; i < thread_count; ++i)
        {
            threads.push_back(std::thread(&thread_pool::worker_thread, this));
        }
    } catch (...)
    {
        done = true;
        throw;
    }
}

void thread_pool::worker_thread()
{
    while (!done)
    {
        function_wrapper task;
        if (work_queue.try_pop(task))
        {
            task();
        }
        else
        {
            std::this_thread::yield();
        }
    }
}