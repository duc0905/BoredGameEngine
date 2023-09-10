#include <gtest/gtest.h>

TEST(ExampleTest, Addition) {
    EXPECT_EQ(2 + 2, 4);

}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();

}

