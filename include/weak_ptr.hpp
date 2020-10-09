#pragma once

#include "shared_ptr.hpp"

namespace stl {
template <typename T>
class weak_ptr {
public:
    weak_ptr() : sptr_(nullptr), cb_(nullptr) {}
    weak_ptr(const weak_ptr& r) : sptr_(r.sptr_), cb_(r.cb_) { cb_->weak_++; }
    weak_ptr(const shared_ptr<T>& r) : sptr_(&r), cb_(r.cb_) { cb_->weak_++; }
    weak_ptr(weak_ptr&& r) : sptr_(r.sptr_), cb_(r.cb_) {
        r.sptr_ = nullptr;
        r.cb_ = nullptr;
    }

    weak_ptr& operator=(const weak_ptr& r) {
        weak_ptr ptr(*this);
        cb_->weak_--;
        sptr_ = r.sptr_;
        cb_ = r.cb_;
        cb_->weak_++;
        return *this;
    }

    weak_ptr& operator=(weak_ptr&& r) {
        weak_ptr ptr(*this);
        cb_->weak_--;
        sptr_ = r.sptr_;
        cb_ = r.cb_;
        r.cb_ = nullptr;
        r.sptr_ = nullptr;
        return *this;
    }

    ~weak_ptr() {
        if (cb_) {
            cb_->weak_--;
            if (cb_->weak_ == 0) delete cb_;
        }
    }

private:
    const shared_ptr<T>* sptr_;
    control_block<T>* cb_;
};
}  // namespace stl