#pragma once

#include <functional>
#include <memory>
#include <type_traits>

namespace stl {

template <typename T>
struct control_block {
    T* data_;                          // 数据
    long ref_;                         // 引用计数
    long weak_;                        // weak_ptr 数量
    std::function<void(T*)> deleter_;  // 删除器

    control_block(T* data, long ref, long weak) : control_block(data, ref, weak, std::default_delete<T>()) {}
    control_block(T* data, long ref, long weak, std::function<void(T*)> d) : data_(data), ref_(ref), weak_(weak), deleter_(d) {}
};

template <typename T>
class weak_ptr;

template <typename T, typename Deleter>
class unique_ptr;

template <typename T>
class shared_ptr;

template <typename T>
class enable_shared_from_this {
public:
    constexpr enable_shared_from_this() noexcept {}
    enable_shared_from_this(const enable_shared_from_this<T>&) noexcept {}

    enable_shared_from_this<T>& operator=(const enable_shared_from_this<T>& other) noexcept { return *this; }

    ~enable_shared_from_this() {}
    shared_ptr<T> shared_from_this() { return ptr_.lock(); }
    shared_ptr<const T> shared_from_this() const { return ptr_.lock(); }

private:
    weak_ptr<T> ptr_;

    friend shared_ptr<T>;
};

template <typename T>
class shared_ptr {
public:
    using element_type = T;
    // Example
    shared_ptr() noexcept : data_(nullptr), cb_(new control_block<element_type>(data_, 0, 0)) {}
    shared_ptr(std::nullptr_t nptr) noexcept : shared_ptr() {}
    explicit shared_ptr(element_type* ptr) : data_(ptr), cb_(new control_block<element_type>(data_, 1, 0)) {
        if constexpr (std::is_base_of<enable_shared_from_this<T>, T>::value) {
            if (ptr) {
                enable_shared_from_this<T>& base = *ptr;
                base.ptr_ = *this;
            }
        }
    }

    template <typename Deleter>
    shared_ptr(element_type* ptr, Deleter d) : data_(ptr), cb_(new control_block<element_type>(data_, 1, 0, d)) {
        if constexpr (std::is_base_of<enable_shared_from_this<T>, T>::value) {
            if (ptr) {
                enable_shared_from_this<T>& base = *ptr;
                base.ptr_ = *this;
            }
        }
    }
    template <typename Deleter>
    shared_ptr(std::nullptr_t ptr, Deleter d) : data_(ptr), cb_(new control_block<element_type>(data_, 0, 0, d)) {}

    explicit shared_ptr(const weak_ptr<T>& r) : shared_ptr(std::move(r.lock())) {}
    template <typename Deleter>
    shared_ptr(unique_ptr<T, Deleter>&& r) : shared_ptr(r.release(), r.get_deleter()) {}

    shared_ptr(const shared_ptr& r) noexcept : data_(r.data_), cb_(r.cb_) {
        cb_->ref_++;
        if constexpr (std::is_base_of<enable_shared_from_this<T>, T>::value) {
            if (data_) {
                enable_shared_from_this<T>& base = *data_;
                base.ptr_ = *this;
            }
        }
    }
    shared_ptr(shared_ptr&& r) noexcept : data_(r.data_), cb_(r.cb_) {
        r.cb_ = new control_block<element_type>(nullptr, 0, 0);
        r.data_ = nullptr;

        if constexpr (std::is_base_of<enable_shared_from_this<T>, T>::value) {
            if (data_) {
                enable_shared_from_this<T>& base = *data_;
                base.ptr_ = *this;
            }
        }
    }
    // ...more

    shared_ptr& operator=(const shared_ptr& ptr) noexcept {
        shared_ptr p(*this);
        cb_->ref_--;
        data_ = ptr.data_;
        cb_ = ptr.cb_;
        cb_->ref_++;
        return *this;
    }

    shared_ptr& operator=(shared_ptr&& ptr) noexcept {
        shared_ptr p(*this);
        cb_->ref_--;
        data_ = ptr.data_;
        cb_ = ptr.cb_;
        ptr.cb_ = new control_block<element_type>(nullptr, 0, 0);
        ptr.data_ = nullptr;
        return *this;
    }

    template <typename Deleter>
    shared_ptr& operator=(std::unique_ptr<T, Deleter>&& r) {
        shared_ptr ptr(std::move(r));
        *this = ptr;

        return *this;
    }

    ~shared_ptr() noexcept {
        cb_->ref_--;
        if (cb_->ref_ <= 0) {
            if (cb_->weak_ <= 0) {
                delete cb_;
                cb_ = nullptr;
            }

            if (cb_ && cb_->deleter_)
                cb_->deleter_(data_);
            else
                delete data_;
        }
    }

    element_type& operator*() const { return *data_; }

    element_type* operator->() const noexcept { return data_; }

    explicit operator bool() const noexcept { return data_ != nullptr; }

    element_type* get() const noexcept { return data_; }

    long use_count() const noexcept { return cb_->ref_; }

private:
    element_type* data_;  // 数据
    control_block<element_type>* cb_;

    friend weak_ptr<T>;
};

}  // namespace stl