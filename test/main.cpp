#include "../src/model.h"
#include "gtest/gtest.h"
#include <mongocxx/instance.hpp>

class pants : public survey::model<pants>
{
public:
    COLLECTION(pants)

    pants() : survey::model<pants>{"test"}
    {
    }
};

TEST(model_test, properties) { 
    pants document;
    ASSERT_EQ("test", document.db());
    ASSERT_EQ("pants", document.collection());
}

int main(int argc, char* argv[])
{
    mongocxx::instance instance{};

    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}
