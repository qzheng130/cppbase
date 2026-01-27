#pragma once

class noncopyable
{
protected:
    noncopyable() = default;
    ~noncopyable() = default;

    // 禁用拷贝
    noncopyable(const noncopyable &) = delete;
    noncopyable &operator=(const noncopyable &) = delete;

    // 允许移动
    noncopyable(noncopyable &&) = default;
    noncopyable &operator=(noncopyable &&) = default;
};
