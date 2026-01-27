#pragma once
#include <thread>
#include <vector>

class join_threads
{
    std::vector<std::thread> &threads;

public:
    // 不需要拷贝线程数组，使用非const引用，析构时等待每个线程退出
    explicit join_threads(std::vector<std::thread> &threads_)
        : threads(threads_)
    {}
    ~join_threads()
    {
        for (unsigned long i = 0; i < threads.size(); ++i) {
            if (threads[i].joinable())
                threads[i].join();
        }
    }
};
