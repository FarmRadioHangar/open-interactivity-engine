#include <gtest/gtest.h>

int main(int argc, char* argv[])
{
//    dotenv::init();

    ::testing::InitGoogleTest(&argc, argv);
    int result = RUN_ALL_TESTS();

    return result;
}
