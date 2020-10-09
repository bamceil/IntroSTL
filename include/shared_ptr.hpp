#pragma once

#include <functional>
#include <memory>

namespace stl {

template <typename T>
struct control_block {
    T* data_;                          // 数据
    size_t ref_;                       // 引用计数
    size_t weak_;                      // weak_ptr 数量
    std::function<void(T*)> deleter_;  // 删除器

    control_block(T* data, size_t ref, size_t weak) : control_block(data, ref, weak, std::default_delete<T>()) {}
    control_block(T* data, size_t ref, size_t weak, std::function<void(T*)> d) : data_(data), ref_(ref), weak_(weak_), deleter_(d) {}
};

template <typename T>
class weak_ptr;

template <typename T>
class shared_ptr {
public:
    using element_type = T;
    // Example
    shared_ptr() : data_(nullptr), cb_(new control_block<element_type>(data_, 0, 0)) {}
    shared_ptr(std::nullptr_t nptr) : shared_ptr() {}
    shared_ptr(element_type* ptr) : data_(ptr), cb_(new control_block<element_type>(data_, 1, 0)) {}
    template <typename Deleter>
    shared_ptr(element_type* ptr, Deleter d) : data_(ptr), cb_(new control_block<element_type>(data_, 1, 0, d)) {}
    // template <typename Deleter>
    // shared_ptr(std::nullptr_t nptr, Deleter d);
    shared_ptr(const shared_ptr& r) : data_(r.data_), cb_(r.cb_) { cb_->ref_++; }
    shared_ptr(shared_ptr&& r) : data_(r.data_), cb_(r.cb_) { r.cb_ = nullptr, r.data_ = nullptr; }
    // ...more

    shared_ptr& operator=(const shared_ptr& ptr) {
        shared_ptr p(*this);
        cb_->ref_--;
        data_ = ptr.data_;
        cb_ = ptr.cb_;
        cb_->ref_++;
        return *this;
    }

    shared_ptr& operator=(shared_ptr&& ptr) {
        shared_ptr p(*this);
        cb_->ref_--;
        data_ = ptr.data_;
        cb_ = ptr.cb_;
        ptr.cb_ = nullptr;
        ptr.data_ = nullptr;
        return *this;
    }

    ~shared_ptr() {
        cb_->ref_--;
        if (cb_->ref_ == 0) {
            cb_->deleter_(data_);
            if (cb_->weak_ == 0) delete cb_;
        }
    }

private:
    element_type* data_;  // 数据
    control_block<element_type>* cb_;

    friend class weak_ptr<T>;
};

}  // namespace stl