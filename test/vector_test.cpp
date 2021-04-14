#include <vector.hpp>

#include <algorithm>
#include <gtest/gtest.h>

using stl::vector;

TEST(vector, basic) {
    vector<int> v(10, 5);
    EXPECT_EQ(v.size(), 10);

    v[0] = 0;
    EXPECT_EQ(v.front(), 0);
    v[9] = 9;
    EXPECT_EQ(v.back(), 9);
    EXPECT_EQ(*v.data(), 0);
    *v.data() = 8;
    EXPECT_EQ(v.front(), 8);

    v.push_back(1);
    EXPECT_EQ(1, v.back());
    EXPECT_EQ(v.size(), 11);
    v.pop_back();
    EXPECT_EQ(v.size(), 10);

    EXPECT_GE(v.capacity(), v.size());

    v.resize(5);
    EXPECT_EQ(v.size(), 5);
}

TEST(vector, iterator) {
    vector<int> v({ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 });
    EXPECT_EQ(v.size(), 10);
    for (int i = 0; i < 10; ++i) EXPECT_EQ(i + 1, *(v.begin() + i));
    vector<int>::iterator it = v.begin();
    EXPECT_EQ(it[9], 10);
    std::sort(v.begin(), v.end(), std::greater<int>());
    EXPECT_EQ(v[0], 10);
    *(v.begin()) = -1;
    EXPECT_EQ(v[0], -1);
}

TEST(vector, reverse_iterator) {
    vector<int> v({ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 });
    int val = 10;
    for (auto it = v.crbegin(); it != v.crend(); ++it) EXPECT_EQ(*it, val--);
}

int main(int argc, char* argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}