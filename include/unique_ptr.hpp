#pragma once

#include <memory>

namespace stl {
template <typename T, typename Deleter = std::default_delete<T>>
class unique_ptr {
public:
    using element_type = T;

    unique_ptr() : data_(nullptr), del_() {}
    unique_ptr(std::nullptr_t) : unique_ptr() {}
    explicit unique_ptr(T* p) : data_(p), del_() {}
    unique_ptr(T* p, const Deleter& d) : data_(p), del_(d) {}
    unique_ptr(unique_ptr&& u) : data_(u.data_), del_(std::move(u.del_)) { u.data_ = nullptr; }
    unique_ptr(const unique_ptr&) = delete;

    ~unique_ptr() {
        if (data_ != nullptr) del_(data_);
    }

    unique_ptr& operator=(const unique_ptr&) = delete;
    unique_ptr& operator=(unique_ptr&& u) {
        delete data_;
        data_ = u.data_;
        del_ = std::move(u.del_);
        u.data_ = nullptr;
    }

private:
    element_type* data_;
    Deleter del_;
};

template <typename T, typename Deleter>
class unique_ptr<T[], Deleter> {
public:
    using element_type = T;

    unique_ptr() : data_(nullptr), del_() {}
    unique_ptr(std::nullptr_t) : unique_ptr() {}
    unique_ptr(element_type* t) : data_(t) {}

private:
    element_type* data_;
    Deleter del_;
};
}  // namespace stl
