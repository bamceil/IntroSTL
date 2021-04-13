#include "shared_ptr.hpp"
#include "weak_ptr.hpp"

#include <gtest/gtest.h>

using stl::shared_ptr;
using stl::weak_ptr;

TEST(weak_ptr, basic) {
    weak_ptr<int> w;
    EXPECT_TRUE(w.expired());

    shared_ptr<int> p(new int(7));
    w = p;
    EXPECT_EQ(p.use_count(), 1);
    EXPECT_FALSE(w.expired());
}

int main(int argc, char* argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}