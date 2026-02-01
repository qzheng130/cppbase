#include "thread_pool.h"
#include "thread_pool_fixedsize.h"
#include <iostream>
#include <latch>
#include <stdio.h>
#include <thread>
#include <unistd.h>

void print()
{
    printf("tid=%d\n", std::this_thread::get_id());
}

void printString(const std::string &str)
{
    std::cout << str << std::endl;
    std::this_thread::sleep_for(std::chrono::microseconds(100));
}

void test(int numThreads)
{
    std::cout << "Test ThreadPool with max queue size = " << numThreads << std::endl;
    thread_pool_fixedsize pool(100, (unsigned) numThreads);
    // thread_pool pool2((unsigned) numThreads);

    pool.submit(print);
    pool.submit(print);
    std::cout << "start loop" << std::endl;

    for (int i = 0; i < 20; ++i) {
        char buf[32];
        snprintf(buf, sizeof buf, "task %d", i);
        pool.submit(std::bind(printString, std::string(buf)));
    }

    std::cout << "caller thread block" << std::endl;

    std::latch latch(1);
    pool.submit(std::bind(&std::latch::count_down, &latch, 1));
    latch.wait();

    std::cout << "caller thread unblock" << std::endl;

    std::cout << "caller thread exit" << std::endl;
}

void longTask(int num)
{
    std::cout << "longTask " << num << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(5000));
}

void test2()
{
    std::cout << "Test ThreadPool by stoping early." << std::endl;
    thread_pool_fixedsize pool(30);

    std::thread thread1([&pool]() {
        for (int i = 0; i < 20; ++i) {
            pool.submit(std::bind(longTask, i));
        }
    });

    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    std::cout << "early stop pool" << std::endl;
    pool.stop();

    // submit() after stop()
    pool.submit(print);
    std::cout << "test2 done" << std::endl;
}

int main()
{
    // test(0);
    test(1);
    // test(5);

    test2();
}
