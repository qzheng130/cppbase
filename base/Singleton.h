#pragma once
#include "Noncopyable.h"
// #include <mutex>
#include <memory>

// Subclass add: friend class Singleton<Subclass>;  // 允许基类调用子类的构造函数
namespace cppbase
{
template <typename T> class Singleton : public Noncopyable
{
public:
    // Singleton(const Singleton &) = delete;
    // Singleton &operator=(const Singleton &) = delete;

    static T &instance()
    {
        static T instance;
        return instance;
    }
};
} // namespace cppbase

// Subclass add: friend class SingletonPtr<Subclass>;  // 允许基类调用子类的构造函数
namespace cppbase
{
template <typename T> class SingletonPtr : public Noncopyable
{
public:
    // SingletonPtr(const SingletonPtr &) = delete;
    // SingletonPtr &operator=(const SingletonPtr &) = delete;

    static std::shared_ptr<T> instance()
    {
        static std::shared_ptr<T> instancePtr = std::make_shared<T>();
        return instancePtr;
    }
};

} // namespace cppbase
