#pragma once

#include <cstddef>
#include <iterator>
#include <stdexcept>
#include <utility>

namespace stl {
template <typename T, std::size_t N>
class array {
public:
    using value_type = T;
    using reference = value_type&;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using const_pointer = const value_type*;
    using iterator = pointer;
    using const_iterator = const_pointer;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;
    static_assert(N != 0, "invalid size 0.");

public:
    reference at(size_type pos) {
        if (pos >= N) throw std::out_of_range("out of range");
        return data_[pos];
    }
    const_reference at(size_type pos) const {
        if (pos >= N) throw std::out_of_range("out of range");
        return data_[pos];
    }
    reference operator[](size_type pos) { return data_[pos]; }
    const_reference operator[](size_type pos) const { return data_[pos]; }
    reference front() { return data_[0]; }
    const_reference front() const { return data_[0]; }
    reference back() { return data_[N - 1]; }
    const_reference back() const { return data_[N - 1]; }
    pointer data() noexcept { return data_; }
    const_pointer data() const noexcept { return data_; }
    constexpr bool empty() const noexcept { return N == 0; }
    constexpr size_type size() const noexcept { return N; }
    constexpr size_type max_size() const noexcept { return N; }
    void fill(const_reference value) {
        for (size_type i = 0; i < N; i++) data_[i] = value;
    }

    iterator begin() noexcept { return data_; }
    const_iterator begin() const noexcept { return data_; }
    const_iterator cbegin() const noexcept { return data_; }
    iterator end() noexcept { return data_ + N; }
    const_iterator end() const noexcept { return data_ + N; }
    const_iterator cend() const noexcept { return data_ + N; }
    reverse_iterator rbegin() noexcept { return std::reverse_iterator(data_ + N); }
    const_reverse_iterator rbegin() const noexcept { return std::reverse_iterator(data_ + N); }
    const_reverse_iterator crbegin() const noexcept { return std::reverse_iterator(data_ + N); }
    reverse_iterator rend() noexcept { return std::reverse_iterator(data_); }
    const_reverse_iterator rend() const noexcept { return std::reverse_iterator(data_); }
    const_reverse_iterator crend() const noexcept { return std::reverse_iterator(data_); }

    void swap(array& other) {
        for (size_type i = 0; i < N; i++) {
            value_type v = data_[i];
            data_[i] = other.data_[i];
            other.data_[i] = v;
        }
    }

public:
    value_type data_[N == 0 ? 1 : N];
};
}  // namespace stl