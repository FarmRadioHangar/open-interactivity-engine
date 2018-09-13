#include "gtest/gtest.h"
#include "../src/model.h"
#include "../src/model/exception.h"
#include "../src/dotenv/dotenv.h"
#include <bsoncxx/builder/basic/document.hpp>
#include <cassert>
#include <cpprest/json.h>
#include <mongocxx/instance.hpp>

class pants : public survey::model<pants>
{
public:
    COLLECTION(pants)

    pants() 
      : survey::model<pants>{
            mongocxx::uri{dotenv::getenv("MONGODB_HOST", "mongodb://localhost:27017")}, 
            dotenv::getenv("MONGODB_DATABASE", "test")} 
    {
    }
};

class model_db_test_fixture : public ::testing::Test
{
protected:
    virtual void SetUp() override
    {
    }

    virtual void TearDown() override
    {
    }
};

class model_page_test_fixture : public ::testing::Test
{
protected:
    virtual void SetUp() override
    {
        using bsoncxx::builder::basic::kvp;

        mongocxx::client client(mongocxx::uri{dotenv::getenv("MONGODB_HOST", 
              "mongodb://localhost:27017")});

        auto db = client.database(dotenv::getenv("MONGODB_DATABASE", "test"));
        db.drop();

        auto collection = db.collection(pants::mongodb_collection);

        uint16_t i;
        for (i = 0; i < 330; i++) 
        {
            bsoncxx::builder::basic::document builder{};
            builder.append(kvp("item", i));
            bsoncxx::document::value data = builder.extract();
            collection.insert_one(data.view());
        }
    }

    virtual void TearDown() override
    {
        mongocxx::client client(mongocxx::uri{dotenv::getenv("MONGODB_HOST", 
              "mongodb://localhost:27017")});

        auto db = client.database(dotenv::getenv("MONGODB_DATABASE", "test"));
        db.drop();
    }
};

/* Test that database and collection names are set when creating a document. */
TEST(model_test, document_initialization)
{
    pants document;

    const auto collection_name = pants::mongodb_collection;

    ASSERT_EQ(dotenv::getenv("MONGODB_DATABASE", "test"), document.db());
    ASSERT_EQ(collection_name, document.collection());
}

/* Test that an ObjectID is generated when creating a document. */
TEST(model_test, oid_is_not_empty)
{
    pants document;

    ASSERT_FALSE(document.oid().empty());
}

/* Test that copy construction behaves as expected w.r.t. data members. */
TEST(model_test, copy_props_matches_original)
{
    pants document;
    pants other(document);

    const auto collection_name = pants::mongodb_collection;

    ASSERT_EQ(document.oid(), other.oid());

    ASSERT_EQ(dotenv::getenv("MONGODB_DATABASE", "test"), other.db());
    ASSERT_EQ(collection_name, other.collection());
}

/* Test that copy assignment behaves as expected w.r.t. data members. */
TEST(model_test, copy_assign_props_matches_original)
{
    pants document;
    pants other;

    const auto collection_name = pants::mongodb_collection;

    other = document;

    ASSERT_EQ(document.oid(), other.oid());

    ASSERT_EQ(dotenv::getenv("MONGODB_DATABASE", "test"), other.db());
    ASSERT_EQ(collection_name, other.collection());
}

/* Test that copy construction and copy assignment copies the document's JSON 
 * payload. */
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

TEST_F(model_db_test_fixture, save_and_get_document)
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

TEST_F(model_db_test_fixture, get_removed_document_throws)
{
    pants document;

    document.save();

    const auto oid = document.oid();

    document.remove();

    ASSERT_THROW(pants::get(oid), survey::model_error);
}

TEST_F(model_db_test_fixture, removed_document_is_empty)
{
    pants document;
    document.set_data(web::json::value::parse("{\"some\":\"string\"}"));

    document.save();
    document.remove();

    ASSERT_EQ(document.data(), web::json::value{});
}

TEST_F(model_page_test_fixture, document_count_is_OK)
{
    ASSERT_EQ(pants::count(), 330);
}

TEST_F(model_page_test_fixture, default_page_size_is_OK)
{
    auto page = pants::page();
    const auto default_limit = pants::default_page_limit;

    ASSERT_EQ(page.count(), default_limit);
}

TEST_F(model_page_test_fixture, page_size_is_OK)
{
    {
        auto page = pants::page(0, 10);

        ASSERT_EQ(page.count(), 10);
        ASSERT_EQ(page.total(), 330);
    }
    {
        auto page = pants::page(0, 30);

        ASSERT_EQ(page.count(), 30);
        ASSERT_EQ(page.total(), 330);
    }
    {
        auto page = pants::page(0, 500);

        ASSERT_EQ(page.count(), 330);
        ASSERT_EQ(page.total(), 330);
    }
}

TEST_F(model_page_test_fixture, pagination)
{
    {
        auto page = pants::page(0, 4);

        ASSERT_EQ(page.at(0)["item"].as_integer(), 0);
        ASSERT_EQ(page.at(3)["item"].as_integer(), 3);
    }
    {
        auto page = pants::page(4, 8);

        ASSERT_EQ(page.at(0)["item"].as_integer(), 4);
        ASSERT_EQ(page.at(3)["item"].as_integer(), 7);
    }
}

int main(int argc, char* argv[])
{
    dotenv::init();
    mongocxx::instance instance{};

    ::testing::InitGoogleTest(&argc, argv);
    int result = RUN_ALL_TESTS();

    return result;
}
