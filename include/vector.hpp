#pragma once

#include <iterator>
#include <memory>
#include <utility>

namespace stl {
template <typename T, typename Allocator = std::allocator<T>>
class vector {
public:
    template <typename Iterator>
    class vector_iter;
    using value_type = T;
    using allocator_type = Allocator;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = typename std::allocator_traits<Allocator>::pointer;
    using const_pointer = typename std::allocator_traits<Allocator>::const_pointer;
    using iterator = vector_iter<pointer>;
    using const_iterator = vector_iter<const_pointer>;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

public:
    vector() noexcept(noexcept(Allocator())) : data_(nullptr), size_(0), cap_(10), allc_(allocator_type()) { data_ = allc_.allocate(cap_); }
    explicit vector(const Allocator& alloc) noexcept : data_(nullptr), size_(0), cap_(10), allc_(alloc) { data_ = allc_.allocate(cap_); }
    vector(size_type count, const T& value, const Allocator& alloc = Allocator()) : data_(nullptr), size_(0), cap_(count), allc_(alloc) {
        data_ = allc_.allocate(cap_);
        for (; size_ < count; ++size_) std::allocator_traits<allocator_type>::construct(allc_, data_ + size_, value);
    }
    explicit vector(size_type count, const Allocator& alloc = Allocator()) : data_(nullptr), size_(0), cap_(count), allc_(alloc) { data_ = allc_.allocate(cap_); }
    template <typename InputIt, typename = typename std::enable_if<std::is_base_of<std::input_iterator_tag, typename std::iterator_traits<InputIt>::iterator_category>::value>::type>
    vector(InputIt first, InputIt last, const Allocator& alloc = Allocator()) : data_(nullptr), size_(0), cap_(std::distance(first, last)), allc_(alloc) {
        data_ = allc_.allocate(cap_);
        for (; first != last; ++first, ++size_) std::allocator_traits<allocator_type>::construct(allc_, data_ + size_, *first);
    }
    vector(const vector& other) : vector(other, other.allc_) {}
    vector(const vector& other, const Allocator& alloc) : data_(nullptr), size_(0), cap_(other.cap_), allc_(alloc) {
        data_ = allc_.allocate(cap_);
        for (size_type i = 0; i < other.size_; i++) std::allocator_traits<allocator_type>::construct(allc_, data_ + i, other.data_[i]);
        size_ = other.size_;
    }
    vector(vector&& other) noexcept : data_(other.data_), size_(other.size_), cap_(other.cap_), allc_(std::move(other.allc_)) {
        other.data_ = nullptr;
        other.cap_ = other.size_ = 0;
    }
    vector(vector&& other, const Allocator& alloc) : data_(other.data_), size_(other.size_), cap_(other.cap_), allc_(alloc) {
        other.data_ = nullptr;
        other.cap_ = other.size_ = 0;
    }
    vector(std::initializer_list<T> init, const Allocator& alloc = Allocator()) : data_(nullptr), size_(0), cap_(init.size()), allc_(alloc) {
        data_ = allc_.allocate(cap_);
        for (auto it = init.begin(); it != init.end(); ++it, ++size_) std::allocator_traits<allocator_type>::construct(allc_, data_ + size_, *it);
    }
    ~vector() {
        destory_element();
        free_space();
    }

public:
    vector& operator=(const vector& other) {
        destory_element();
        free_space();
        vector v(other);
        swap(v);
        return *this;
    }

    vector& operator=(vector&& other) {
        destory_element();
        free_space();
        swap(other);
        return *this;
    }

    vector& operator=(std::initializer_list<T> ilist) {
        destory_element();
        free_space();
        vector v(ilist);
        swap(v);
        return *this;
    }

public:
    void swap(vector& other) {
        std::swap(other.allc_, allc_);
        std::swap(other.cap_, cap_);
        std::swap(other.data_, data_);
        std::swap(other.size_, size_);
    }
    void assign(size_type count, const T& value) {
        vector v(count, value);
        swap(v);
    }
    template <class InputIt, typename = typename std::enable_if<std::is_base_of<std::input_iterator_tag, typename std::iterator_traits<InputIt>::iterator_category>::value>::type>
    void assign(InputIt first, InputIt last) {
        vector v(first, last);
        swap(v);
    }
    void assign(std::initializer_list<T> ilist) {
        vector v(ilist);
        swap(v);
    }

    allocator_type get_allocator() const noexcept { return allc_; }
    reference at(size_type pos) {
        check_bound(pos);
        return data_[pos];
    }
    const_reference at(size_type pos) const {
        check_bound(pos);
        return data_[pos];
    }
    reference operator[](size_type pos) { return data_[pos]; }
    const_reference operator[](size_type pos) const { return data_[pos]; }
    reference front() { return data_[0]; }
    const_reference front() const { return data_[0]; }
    reference back() { return data_[size_ - 1]; }
    const_reference back() const { return data_[size_ - 1]; }
    T* data() noexcept { return data_; }
    const T* data() const noexcept { return data_; }
    bool empty() const noexcept { return size_ == 0; }
    size_type size() const noexcept { return size_; }
    size_type capacity() const noexcept { return cap_; }
    // size_type max_size() const noexcept { return 0; }
    void reserve(size_type new_cap) {
        if (new_cap <= cap_) return;
        pointer newp = allc_.allocate(new_cap);
        size_type old_size = size_;
        for (size_type i = 0; i < size_; ++i) std::allocator_traits<allocator_type>::construct(allc_, newp + i, std::move(data_[i]));
        destory_element();
        free_space();
        cap_ = new_cap;
        size_ = old_size;
        data_ = newp;
    }
    void shrink_to_fit() {}
    void clear() { destory_element(); }
    void push_back(const value_type& value) {
        if (cap_ == size_) reserve(1.5 * cap_);
        std::allocator_traits<allocator_type>::construct(allc_, data_ + size_, value);
        ++size_;
    }
    void push_back(T&& value) {
        if (cap_ == size_) reserve(1.5 * cap_);
        std::allocator_traits<allocator_type>::construct(allc_, data_ + size_, std::move(value));
        ++size_;
    }
    template <typename... Args>
    reference emplace_back(Args&&... args) {
        if (cap_ == size_) reserve(1.5 * cap_);
        std::allocator_traits<allocator_type>::construct(allc_, data_ + size_, std::forward<Args>(args)...);
        ++size_;
        return data_[size_ - 1];
    }
    void pop_back() {
        if (size_ == 0) throw std::out_of_range("out of range.");
        std::allocator_traits<allocator_type>::destroy(allc_, data_ + size_ - 1);
        --size_;
    }
    void resize(size_type count) {
        if (cap_ < count) reserve(count);
        for (; size_ < count; ++size_) std::allocator_traits<allocator_type>::construct(allc_, data_ + size_);
        if (size_ == count) return;
        for (; size_ > count; --size_) std::allocator_traits<allocator_type>::destroy(allc_, data_ + size_ - 1);
    }

    iterator begin() noexcept { return iterator(data_); }
    const_iterator begin() const noexcept { return const_iterator(data_); }
    const_iterator cbegin() const noexcept { return const_iterator(data_); }
    iterator end() noexcept { return iterator(data_ + size_); }
    const_iterator end() const noexcept { return const_iterator(data_ + size_); }
    const_iterator cend() const noexcept { return const_iterator(data_ + size_); }
    reverse_iterator rbegin() noexcept { return reverse_iterator(end()); }
    const_reverse_iterator rbegin() const noexcept { return const_reverse_iterator(end()); }
    const_reverse_iterator crbegin() const noexcept { return const_reverse_iterator(cend()); }
    reverse_iterator rend() noexcept { return reverse_iterator(begin()); }
    const_reverse_iterator rend() const noexcept { return const_reverse_iterator(begin()); }
    const_reverse_iterator crend() const noexcept { return const_reverse_iterator(cbegin()); }

private:
    pointer data_;
    size_type size_;
    size_type cap_;
    allocator_type allc_;

private:
    void destory_element() {
        for (size_type i = 0; i < size_; ++i) std::allocator_traits<allocator_type>::destroy(allc_, data_ + i);
        size_ = 0;
    }

    void free_space() {
        allc_.deallocate(data_, cap_);
        data_ = nullptr;
        cap_ = 0;
    }

    void check_bound(size_type pos) {
        if (pos >= size_) throw std::out_of_range("out of range.");
    }
};

template <typename T, typename Allocator>
template <typename Iter>
class vector<T, Allocator>::vector_iter {
public:
    using value_type = typename std::iterator_traits<Iter>::value_type;
    using difference_type = typename std::iterator_traits<Iter>::difference_type;
    using pointer = typename std::iterator_traits<Iter>::pointer;
    using reference = typename std::iterator_traits<Iter>::reference;
    using iterator_category = std::random_access_iterator_tag;

public:
    explicit vector_iter(pointer p) : current_(p) {}

public:
    vector_iter& operator++() {
        ++current_;
        return *this;
    }
    vector_iter& operator++(int) { return vector_iter(current_++); }
    vector_iter& operator--() {
        --current_;
        return *this;
    }
    vector_iter& operator--(int) { return vector_iter(current_--); }

    reference operator*() const { return *current_; }

    pointer operator->() const { return current_; }

    reference operator[](difference_type n) const { return current_[n]; }

    vector_iter& operator+=(difference_type n) {
        current_ += n;
        return *this;
    }
    vector_iter& operator-=(difference_type n) {
        current_ -= n;
        return *this;
    }
    vector_iter operator+(difference_type n) const { return vector_iter(current_ + n); }
    vector_iter operator-(difference_type n) const { return vector_iter(current_ - n); }
    const pointer base() const { return current_; }

private:
    pointer current_;
};

template <typename Iter>
inline bool operator==(const Iter& itl, const Iter& itr) {
    return itl.base() == itr.base();
}

template <typename Iter>
inline bool operator!=(const Iter& itl, const Iter& itr) {
    return itl.base() != itr.base();
}

template <typename Iter>
inline bool operator<(const Iter& itl, const Iter& itr) {
    return itl.base() < itr.base();
}

template <typename Iter>
inline bool operator>(const Iter& itl, const Iter& itr) {
    return itl.base() > itr.base();
}

template <typename Iter>
inline bool operator>=(const Iter& itl, const Iter& itr) {
    return itl.base() >= itr.base();
}

template <typename Iter>
inline bool operator<=(const Iter& itl, const Iter& itr) {
    return itl.base() <= itr.base();
}

template <typename Iter>
inline auto operator-(const Iter& itl, const Iter& itr) -> decltype(itl.base() - itr.base()) {
    return itl.base() - itr.base();
}

template <typename Iter>
inline Iter operator+(typename Iter::difference_type n, const Iter& it) {
    return Iter(it.base() + n);
}

}  // namespace stl