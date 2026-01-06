#pragma once
#include "Noncopyable.h"
#include <mutex>
#include <memory>

// Subclass add: friend class SingletonPtr<Subclass>;  // 允许基类调用子类的构造函数

namespace cppbase
{
template <typename T> class SingletonPtr : Noncopyable
{
public:
    SingletonPtr(const SingletonPtr &) = delete;
    SingletonPtr &operator=(const SingletonPtr &) = delete;

    static std::shared_ptr<T> instance()
    {
        static std::shared_ptr<T> instancePtr = std::make_shared<T>();
        return instancePtr;
    }
};

// Subclass add: friend class Singleton<Subclass>;  // 允许基类调用子类的构造函数
template <typename T> class Singleton : Noncopyable
{
public:
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;

    static T& instance()
    {
        static T instance;
        return instance;
    }
};

// Subclass add: friend class SingletonTemplate<Subclass>;  // 允许基类调用子类的构造函数
template <typename T> class SingletonTemplate : Noncopyable
{
public:
    static std::shared_ptr<T> instance()
    {
        if (!instance_) {
            std::lock_guard<std::mutex> lock(mutex_);
            if (!instance_) {
                instance_ = std::make_shared<T>();
            }
        }
        return instance_;
    }

protected:
    static std::mutex mutex_;
    static std::shared_ptr<T> instance_;
};

template <typename T> std::mutex SingletonTemplate<T>::mutex_;

template <typename T>
std::shared_ptr<T> SingletonTemplate<T>::instance_;
} // namespace cppbase
