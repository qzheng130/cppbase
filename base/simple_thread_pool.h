#include "join_threads.h"
#include "threadsafe_queue.h"
#include <atomic>
#include <functional>
#include <thread>
#include <vector>

class simple_thread_pool
{
public:
    simple_thread_pool()
        : done(false)
        , joiner(threads)
    {
        // 线程数量
        unsigned const thread_count=std::thread::hardware_concurrency();
        try
        {
            for(unsigned i=0;i<thread_count;++i)
            {
                threads.push_back(std::thread(&simple_thread_pool::worker_thread, this));
            }
        }
        catch(...)
        {
            // 异常发生，退出已经创建的线程
            done=true;
            throw;
        }
    }

    ~simple_thread_pool() { done = true; }

    template<typename FunctionType>
    void submit(FunctionType f)
    {
        work_queue.push(std::function<void()>(f));
    }

private:
    void worker_thread()
    {
        while(!done)
        {
            std::function<void()> task;
            if(work_queue.try_pop(task))
            {
                task();
            }
            else
            {
                // 如果任务队列中没有任务，让出 CPU 给其他线程机会
                // 只是建议调度器切换线程
                std::this_thread::yield();
            }
        }
    }
    
    std::atomic_bool done;
    threadsafe_queue<std::function<void()>> work_queue;
    std::vector<std::thread> threads;
    // 保证线程池析构/销毁前，所有的线程都执行完毕
    join_threads joiner;
};
