#include <unique_ptr.hpp>

#include <gtest/gtest.h>

using stl::unique_ptr;

class A {
public:
    explicit A(int k) : K(k) {}

    int value() const { return K; }

private:
    int K;
};

TEST(unique_ptr, basic) {
    unique_ptr<int> p(new int(5));
    EXPECT_EQ(5, *p);

    *p = 12;
    EXPECT_EQ(12, *p);

    unique_ptr<int> p2(new int(100));
    EXPECT_TRUE(p2);

    p = std::move(p2);
    EXPECT_TRUE(p);
    EXPECT_FALSE(p2);
    EXPECT_EQ(*p, 100);

    p = nullptr;
    EXPECT_FALSE(p);
}

TEST(unique_ptr, release) {
    unique_ptr<int> p(new int(5));

    int* ptr = p.get();
    EXPECT_EQ(*ptr, *p);
    EXPECT_EQ(*ptr, 5);

    int* p1 = p.release();
    EXPECT_FALSE(p);
    EXPECT_EQ(*p1, 5);
    delete p1;
}

TEST(unique_ptr, object) {
    unique_ptr<A> p(new A(12));
    EXPECT_TRUE(p);

    EXPECT_EQ(p->value(), 12);
}

TEST(unique_ptr_array, basic) {
    unique_ptr<int[]> p(new int[2]);
    p[0] = 1;
    p[1] = 2;
    EXPECT_TRUE(p);

    EXPECT_EQ(1, p[0]);
    EXPECT_EQ(2, p[1]);
}

TEST(unique_ptr_array, move) {
    unique_ptr<int[]> p(new int[2]);
    p[0] = 1;
    p[1] = 2;

    unique_ptr<int[]> p2(std::move(p));
    EXPECT_FALSE(p);

    EXPECT_EQ(1, p2[0]);
    EXPECT_EQ(2, p2[1]);
}

int main(int argc, char* argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}