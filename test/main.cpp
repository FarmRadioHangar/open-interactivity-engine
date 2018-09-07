#include "gtest/gtest.h"
#include "../src/model.h"
#include "../src/model/exception.h"
#include <mongocxx/instance.hpp>
#include <cpprest/json.h>

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

TEST(model_test, copy_props_matches_original)
{
    pants document;
    pants other(document);

    ASSERT_EQ(document.oid(), other.oid());

    ASSERT_EQ("test", other.db());
    ASSERT_EQ("pants", other.collection());
}

TEST(model_test, copy_assign_props_matches_original)
{
    pants document;
    pants other;

    other = document;

    ASSERT_EQ(document.oid(), other.oid());

    ASSERT_EQ("test", other.db());
    ASSERT_EQ("pants", other.collection());
}

TEST(model_test, copy_data_matches_original)
{
    auto data = web::json::value::parse("{\"some\":\"string\"}");

    {
        pants document;
        document.set_data(data);

        pants other(document);

        ASSERT_EQ(document.data(), other.data());
    }
    {
        pants document;
        pants other;

        other = document;

        ASSERT_EQ(document.data(), other.data());
    }
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

TEST(model_test, set_oid)
{
    pants document{};
    bsoncxx::oid oid{};

    const std::string valid_oid(oid.to_string());

    document.set_oid(valid_oid);

    ASSERT_EQ(valid_oid, document.oid());
}

TEST(model_test, save_and_get_document)
{
    pants document{};

    auto data = web::json::value::parse("{\"this\":\"prop\"}");
    document.set_data(data);
    document.save();

    const std::string oid = document.oid();

    pants other = pants::get(oid);

    ASSERT_EQ(oid, other.oid());
    ASSERT_TRUE(other.data()["this"].is_string());
    ASSERT_EQ("prop", other.data()["this"].as_string());
}

TEST(model_test, set_non_object_throws)
{
    pants document;
    const auto json_array = web::json::value::parse("[1,2,3]");

    ASSERT_THROW(document.set_data(json_array), survey::model_error);

    try {
        document.set_data(json_array);
    } catch(const survey::model_error& e) {
        ASSERT_EQ(survey::model_error::bad_bson_data, e.type());
    }

    ASSERT_THROW(document.set_data(std::move(json_array)), survey::model_error);
}

TEST(model_test, get_removed_document_throws)
{
    pants document;

    document.save();

    const auto oid = document.oid();

    document.remove();

    ASSERT_THROW(pants::get(oid), survey::model_error);
}

TEST(model_test, removed_document_is_empty)
{
    pants document;
    document.set_data(web::json::value::parse("{\"some\":\"string\"}"));

    document.save();
    document.remove();

    ASSERT_EQ(document.data(), web::json::value{});
}

int main(int argc, char* argv[])
{
    mongocxx::instance instance{};

    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}
