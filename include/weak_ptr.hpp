#pragma once

#include "shared_ptr.hpp"

namespace stl {
template <typename T>
class weak_ptr {
public:
    weak_ptr() noexcept : sptr_(nullptr), cb_(nullptr) {}
    weak_ptr(const weak_ptr& r) noexcept : sptr_(r.sptr_), cb_(r.cb_) {
        if (cb_) cb_->weak_++;
    }
    weak_ptr(const shared_ptr<T>& r) noexcept : sptr_(&r), cb_(r.cb_) {
        if (cb_) cb_->weak_++;
    }
    weak_ptr(weak_ptr&& r) noexcept : sptr_(r.sptr_), cb_(r.cb_) {
        r.sptr_ = nullptr;
        r.cb_ = nullptr;
    }

    weak_ptr& operator=(const weak_ptr& r) noexcept {
        weak_ptr ptr(*this);
        if (cb_) cb_->weak_--;
        sptr_ = r.sptr_;
        cb_ = r.cb_;
        if (cb_) cb_->weak_++;
        return *this;
    }

    weak_ptr& operator=(weak_ptr&& r) noexcept {
        weak_ptr ptr(*this);
        if (cb_) cb_->weak_--;
        sptr_ = r.sptr_;
        cb_ = r.cb_;
        r.cb_ = nullptr;
        r.sptr_ = nullptr;
        return *this;
    }

    weak_ptr& operator=(const shared_ptr<T>& r) noexcept {
        weak_ptr ptr(*this);
        if (cb_) cb_->weak_--;
        sptr_ = &r;
        cb_ = r.cb_;
        if (cb_) cb_->weak_++;

        return *this;
    }

    ~weak_ptr() noexcept {
        if (cb_) {
            cb_->weak_--;
            if (cb_->weak_ <= 0 && cb_->ref_ <= 0) delete cb_;
        }
    }

    shared_ptr<T> lock() const noexcept {
        if (sptr_)
            return *sptr_;
        else
            return nullptr;
    }

    long use_count() const noexcept {
        if (cb_) return cb_->ref_;
        return 0;
    }

    bool expired() const noexcept { return use_count() == 0; }

private:
    const shared_ptr<T>* sptr_;
    control_block<T>* cb_;
};
}  // namespace stl