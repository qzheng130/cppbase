#pragma once

namespace cppbase
{
class Noncopyable
{
protected:
    Noncopyable() = default;
    ~Noncopyable() = default;

    // 禁用拷贝
    Noncopyable(const Noncopyable &) = delete;
    Noncopyable &operator=(const Noncopyable &) = delete;

    // 允许移动
    Noncopyable(Noncopyable &&) = default;
    Noncopyable &operator=(Noncopyable &&) = default;
};
} // namespace cppbase
