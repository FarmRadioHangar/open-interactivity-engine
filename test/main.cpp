#include "../src/model.h"
#include "../src/model/exception.h"
#include "gtest/gtest.h"
#include <mongocxx/instance.hpp>

class pants : public survey::model<pants>
{
public:
    COLLECTION(pants)

    pants() : survey::model<pants>{"test"} {}
};

TEST(model_test, document_initialization) 
{ 
    pants document;

    ASSERT_EQ("test", document.db());
    ASSERT_EQ("pants", document.collection());
}

TEST(model_test, oid_is_not_empty) 
{
    pants document;

    ASSERT_FALSE(document.oid().empty());
}

TEST(model_test, copy_matches_original) 
{
    pants document;
    pants other(document);

    ASSERT_EQ(document.oid(), other.oid());

    ASSERT_EQ("test", other.db());
    ASSERT_EQ("pants", other.collection());
}

TEST(model_test, copy_assign_matches_original) 
{
    pants document;
    pants other;

    other = document;

    ASSERT_EQ(document.oid(), other.oid());

    ASSERT_EQ("test", other.db());
    ASSERT_EQ("pants", other.collection());
}

TEST(model_test, set_bad_oid_throws) 
{
    pants document;

    ASSERT_THROW(document.set_oid("xxx"), survey::model_error);

    try {
        document.set_oid("xxx");
    } catch(const survey::model_error& e) {
        ASSERT_EQ(survey::model_error::bad_oid, e.type());
    }
}

int main(int argc, char* argv[])
{
    mongocxx::instance instance{};

    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}
