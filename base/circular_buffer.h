#pragma once
#include <algorithm>
#include <deque>
#include <stdexcept>

// replace boost::circular_buffer
const int kQueueCapacity = 10;

template<typename T>
class circular_buffer
{
private:
    std::deque<T> buffer_;
    size_t capacity_;

public:
    explicit circular_buffer(size_t capacity)
        : capacity_(capacity > 0 ? capacity : 1)
    {}

    // circular_buffer()
    //     : circular_buffer(kQueueCapacity)
    // {}

    bool empty() const { return buffer_.empty(); }

    bool full() const { return buffer_.size() == capacity_; }

    size_t capacity() const { return capacity_; }

    size_t size() const { return buffer_.size(); }

    // 向缓冲区添加元素（覆盖最旧的元素）
    void push(const T &value)
    {
        if (buffer_.size() == capacity_) {
            // 缓冲区已满，移除最旧的元素
            buffer_.pop_front();
        }
        buffer_.push_back(value);
    }

    // 使用移动语义添加元素
    void push(T &&value)
    {
        if (buffer_.size() == capacity_) {
            buffer_.pop_front();
        }
        buffer_.push_back(std::move(value));
    }

    // 弹出最旧的元素
    T pop()
    {
        if (empty()) {
            throw std::runtime_error("Buffer is empty");
        }

        T value = std::move(buffer_.front());
        buffer_.pop_front();
        return value;
    }

    // 查看最旧的元素（不弹出）
    const T &front() const
    {
        if (empty()) {
            throw std::runtime_error("Buffer is empty");
        }
        return buffer_.front();
    }

    T &front()
    {
        if (empty()) {
            throw std::runtime_error("Buffer is empty");
        }
        return buffer_.front();
    }

    // 查看最新的元素
    const T &back() const
    {
        if (empty()) {
            throw std::runtime_error("Buffer is empty");
        }
        return buffer_.back();
    }

    T &back()
    {
        if (empty()) {
            throw std::runtime_error("Buffer is empty");
        }
        return buffer_.back();
    }

    // 随机访问（0 = 最旧的元素）
    const T &operator[](size_t index) const
    {
        if (index >= size()) {
            throw std::out_of_range("Index out of range");
        }
        return buffer_[index];
    }

    T &operator[](size_t index)
    {
        if (index >= size()) {
            throw std::out_of_range("Index out of range");
        }
        return buffer_[index];
    }

    void clear() { buffer_.clear(); }

    void set_capacity(size_t new_capacity)
    {
        if (new_capacity == 0)
            new_capacity = 1;

        if (new_capacity < buffer_.size()) {
            // 如果新容量小于当前大小，需要移除一些元素
            size_t elements_to_remove = buffer_.size() - new_capacity;
            buffer_.erase(buffer_.begin(), buffer_.begin() + elements_to_remove);
        }

        capacity_ = new_capacity;
    }

    bool contains(const T &value) const
    {
        return std::find(buffer_.begin(), buffer_.end(), value) != buffer_.end();
    }

    using iterator = typename std::deque<T>::iterator;
    using const_iterator = typename std::deque<T>::const_iterator;

    iterator begin() { return buffer_.begin(); }
    iterator end() { return buffer_.end(); }
    const_iterator begin() const { return buffer_.begin(); }
    const_iterator end() const { return buffer_.end(); }
    const_iterator cbegin() const { return buffer_.cbegin(); }
    const_iterator cend() const { return buffer_.cend(); }

    using reverse_iterator = typename std::deque<T>::reverse_iterator;
    using const_reverse_iterator = typename std::deque<T>::const_reverse_iterator;

    reverse_iterator rbegin() { return buffer_.rbegin(); }
    reverse_iterator rend() { return buffer_.rend(); }
    const_reverse_iterator rbegin() const { return buffer_.rbegin(); }
    const_reverse_iterator rend() const { return buffer_.rend(); }
    const_reverse_iterator crbegin() const { return buffer_.crbegin(); }
    const_reverse_iterator crend() const { return buffer_.crend(); }
};
