#pragma once

#include <memory>

namespace stl {
template <typename T, typename Deleter = std::default_delete<T>>
class unique_ptr {
public:
    using element_type = T;
    using pointer = T*;

    unique_ptr() noexcept : data_(nullptr), del_() {}
    unique_ptr(std::nullptr_t) noexcept : unique_ptr() {}
    explicit unique_ptr(pointer p) noexcept : data_(p), del_() {}
    unique_ptr(pointer p, const Deleter& d) noexcept : data_(p), del_(d) {}
    unique_ptr(unique_ptr&& u) noexcept : data_(u.data_), del_(std::move(u.del_)) { u.data_ = nullptr; }
    unique_ptr(const unique_ptr&) = delete;

    ~unique_ptr() noexcept { del_(data_); }

    unique_ptr& operator=(const unique_ptr&) = delete;
    unique_ptr& operator=(unique_ptr&& u) {
        del_(data_);
        data_ = u.data_;
        del_ = std::move(u.del_);
        u.data_ = nullptr;
        return *this;
    }
    unique_ptr& operator=(std::nullptr_t) noexcept {
        del_(data_);
        data_ = nullptr;
        return *this;
    }

    element_type& operator*() const { return *data_; }

    pointer operator->() const noexcept { return data_; }

    explicit operator bool() const noexcept { return data_ != nullptr; }

    pointer release() noexcept {
        pointer ret = data_;
        data_ = nullptr;
        return ret;
    }

    pointer get() const noexcept { return data_; }

    Deleter& get_deleter() noexcept { return del_; }

    const Deleter& get_deleter() const noexcept { return del_; }

private:
    pointer data_;
    Deleter del_;
};

template <typename T, typename Deleter>
class unique_ptr<T[], Deleter> {
public:
    using element_type = T;
    using pointer = T*;

    unique_ptr() noexcept : data_(nullptr), del_() {}
    unique_ptr(std::nullptr_t) noexcept : unique_ptr() {}
    explicit unique_ptr(pointer t) noexcept : data_(t) {}
    unique_ptr(unique_ptr&& u) noexcept : data_(u.data_), del_(std::move(u.del_)) { u.data_ = nullptr; }
    unique_ptr(const unique_ptr&) = delete;
    ~unique_ptr() noexcept { del_(data_); }

    unique_ptr& operator=(const unique_ptr&) = delete;
    unique_ptr& operator=(unique_ptr&& u) {
        del_(data_);
        data_ = u.data_;
        del_ = std::move(u.del_);
        u.data_ = nullptr;
        return *this;
    }
    unique_ptr& operator=(std::nullptr_t) noexcept {
        del_(data_);
        data_ = nullptr;
        return *this;
    }

    explicit operator bool() const noexcept { return data_ != nullptr; }

    element_type& operator[](std::size_t i) const { return data_[i]; }

    pointer release() noexcept {
        pointer ret = data_;
        data_ = nullptr;
        return ret;
    }

    pointer get() const noexcept { return data_; }

    Deleter& get_deleter() noexcept { return del_; }

    const Deleter& get_deleter() const noexcept { return del_; }

private:
    pointer data_;
    Deleter del_;
};
}  // namespace stl
