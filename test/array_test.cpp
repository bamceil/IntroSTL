#include <array.hpp>

#include <algorithm>
#include <gtest/gtest.h>

using stl::array;

TEST(array, basic) {
    array<int, 10> arr = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
    EXPECT_EQ(arr[0], 1);
    EXPECT_EQ(arr.at(4), 5);
    EXPECT_EQ(arr.back(), 10);
    EXPECT_EQ(arr.front(), 1);

    arr[0] = 100;
    EXPECT_EQ(arr.front(), 100);
    EXPECT_EQ(arr.size(), 10);
    EXPECT_EQ(arr.max_size(), 10);
    EXPECT_FALSE(arr.empty());

    auto p = arr.data();
    EXPECT_EQ(*p, 100);
    *p = -1;
    EXPECT_EQ(arr.front(), -1);

    arr.fill(5);
    EXPECT_EQ(arr[6], 5);

    array<int, 10> ap;
    ap.fill(10);

    arr.swap(ap);
    EXPECT_EQ(arr[0], 10);
}

TEST(array, iterator) {
    array<int, 10> arr = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
    int i = 0;
    for (auto iter = arr.begin(); iter != arr.end(); iter++) EXPECT_EQ(*iter, ++i);

    for (auto iter = arr.rbegin(); iter != arr.rend(); iter++) EXPECT_EQ(*iter, i--);
}

TEST(array, algo) {
    array<int, 10> arr = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
    std::sort(arr.begin(), arr.end(), std::greater<int>());
    int i = 10;
    for (int v : arr) EXPECT_EQ(i--, v);
}

int main(int argc, char* argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}