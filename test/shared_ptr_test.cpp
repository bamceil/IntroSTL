#include <shared_ptr.hpp>
#include <unique_ptr.hpp>
#include <weak_ptr.hpp>

#include <gtest/gtest.h>

using stl::enable_shared_from_this;
using stl::shared_ptr;
using stl::unique_ptr;
using stl::weak_ptr;

class A {
public:
    explicit A(int k) : K(k) {}

    int value() const { return K; }

private:
    int K;
};

TEST(shared_ptr, basic) {
    shared_ptr<int> p(new int(3));
    EXPECT_TRUE(p);
    EXPECT_EQ(*p, 3);

    int* ptr = p.get();
    EXPECT_EQ(*ptr, 3);

    auto p1 = p;
    EXPECT_EQ(p1.use_count(), 2);

    p1 = nullptr;
    EXPECT_FALSE(p1);
    EXPECT_EQ(p.use_count(), 1);
}

TEST(shared_ptr, object) {
    shared_ptr<A> p(new A(3));
    EXPECT_TRUE(p);
    EXPECT_EQ(p->value(), 3);
}

TEST(shared_ptr, from_weak) {
    shared_ptr<int> p(new int(3));
    weak_ptr<int> pw(p);
    EXPECT_EQ(pw.use_count(), 1);
    EXPECT_EQ(p.use_count(), 1);

    auto ptr = pw.lock();
    EXPECT_EQ(pw.use_count(), 2);
    EXPECT_EQ(p.use_count(), 2);

    shared_ptr<int> fp(pw);
    EXPECT_EQ(pw.use_count(), 3);
    EXPECT_EQ(p.use_count(), 3);
}

TEST(shared_ptr, from_unique) {
    unique_ptr<int> up(new int(4));
    shared_ptr<int> p(std::move(up));
    EXPECT_FALSE(up);
    EXPECT_EQ(p.use_count(), 1);

    up = unique_ptr<int>(new int(10));
    p = std::move(up);
    EXPECT_EQ(*p, 10);
    EXPECT_FALSE(up);
    EXPECT_EQ(p.use_count(), 1);
}

class shared : public enable_shared_from_this<shared> {
public:
    shared(int y) : t_(y) {}

private:
    int t_;
};

TEST(shared_ptr, enable_shared) {
    shared_ptr<shared> ptr(new shared(3));
    EXPECT_EQ(ptr.use_count(), 1);

    shared_ptr<shared> p = ptr->shared_from_this();
    EXPECT_EQ(ptr.use_count(), 2);
}

int main(int argc, char* argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
