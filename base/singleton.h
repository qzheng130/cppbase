#pragma once
#include "noncopyable.h"
#include <memory>

// Subclass add: friend class singleton<Subclass>;  // 允许基类调用子类的构造函数
template<typename T>
class singleton : public noncopyable
{
public:
    // singleton(const singleton &) = delete;
    // singleton &operator=(const singleton &) = delete;

    static T &instance()
    {
        static T instance;
        return instance;
    }
};

// Subclass add: friend class singleton_ptr<Subclass>;  // 允许基类调用子类的构造函数
template<typename T>
class singleton_ptr : public noncopyable
{
public:
    // singleton_ptr(const singleton_ptr &) = delete;
    // singleton_ptr &operator=(const singleton_ptr &) = delete;

    static std::shared_ptr<T> instance()
    {
        static std::shared_ptr<T> instancePtr = std::make_shared<T>();
        return instancePtr;
    }
};
