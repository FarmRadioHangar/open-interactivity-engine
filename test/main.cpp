#include "gtest/gtest.h"
#include "../src/dotenv/dotenv.h"
#include "../src/model.h"
#include "../src/model/exception.h"
#include "../src/model/validators/property_validator.h"
#include "../src/model/validators/property_validator.h"
#include "../src/model/validators/unique_constraint.h"
#include "../src/model/validators/unique_constraint.h"
#include <bsoncxx/builder/basic/document.hpp>
#include <cpprest/json.h>
#include <mongocxx/instance.hpp>
#include <vector>

class pants : public ops::model<pants>
{
public:
    COLLECTION(pants)

    pants()
      : ops::model<pants>{
            mongocxx::uri{dotenv::getenv("MONGODB_HOST", "mongodb://localhost:27017")},
            dotenv::getenv("MONGODB_DATABASE", "test")}
    {
    }
};

class model_db_test_fixture : public ::testing::Test
{
public:
    std::string id(const std::size_t pos) const { return _ids.at(pos); }

protected:
    virtual void SetUp() override
    {
        using bsoncxx::builder::basic::kvp;

        mongocxx::client client(mongocxx::uri{dotenv::getenv(
              "MONGODB_HOST",
              "mongodb://localhost:27017"
        )});

        auto db = client.database(dotenv::getenv("MONGODB_DATABASE", "test"));
        db.drop();

        auto collection = db.collection(pants::mongodb_collection);

        uint16_t i;
        for (i = 0; i < 330; i++)
        {
            bsoncxx::builder::basic::document builder{};
            builder.append(kvp("item", i));
            bsoncxx::document::value data = builder.extract();
            auto result = collection.insert_one(data.view());
            auto view = result.value().inserted_id().get_oid().value;
            _ids.push_back(view.to_string());
        }
    }

    virtual void TearDown() override
    {
        mongocxx::client client(mongocxx::uri{dotenv::getenv("MONGODB_HOST",
              "mongodb://localhost:27017")});

        auto db = client.database(dotenv::getenv("MONGODB_DATABASE", "test"));
        db.drop();
    }

private:
    std::vector<std::string> _ids;
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

/* Test that copy construction behaves as expected w.r.t. data member
 * initialization. 
 */
TEST(model_test, copy_props_matches_original)
{
    pants document;
    pants other(document);

    const auto collection_name = pants::mongodb_collection;

    ASSERT_EQ(document.oid(), other.oid());

    ASSERT_EQ(dotenv::getenv("MONGODB_DATABASE", "test"), other.db());
    ASSERT_EQ(collection_name, other.collection());
}

/* Test that copy assignment behaves as expected w.r.t. data member
 * initialization. 
 */
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

/* Test that copy construction and copy assignment duplicates the document's
 * JSON payload. 
 */
TEST(model_test, copy_data_matches_original)
{
    const auto data = "{\"some\":\"string\"}";

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

/* Assigning an invalid ObjectID throws an exception. */
TEST(model_test, set_bad_oid_throws)
{
    pants document;

    ASSERT_THROW(document.set_oid("xxx"), ops::model_error);

    try {
        document.set_oid("xxx");
    } catch(const ops::model_error& e) {
        ASSERT_EQ(ops::model_error::bad_oid, e.type());
    }
}

/* Test assignment of a valid ObjectID. */
TEST(model_test, set_oid)
{
    pants document{};
    bsoncxx::oid oid{};

    const std::string valid_oid(oid.to_string());

    document.set_oid(valid_oid);

    ASSERT_EQ(valid_oid, document.oid());
}

/* Save and subsequently retrieve a document from the database. */
TEST_F(model_db_test_fixture, save_and_get_document)
{
    pants document{};

    document.set_data("{\"this\":\"prop\"}");
    document.save();

    const std::string oid = document.oid();

    pants other = pants::get(oid);

    ASSERT_EQ(oid, other.oid());
    ASSERT_TRUE(other.to_json()["this"].is_string());
    ASSERT_EQ("prop", other.to_json()["this"].as_string());
}

/* Retreive, update and subsequently save a document to the database. */
TEST_F(model_db_test_fixture, update_document)
{
    {
        auto document = pants::get(id(0));

        document.set_data("{\"some\":\"string\"}");
        document.save();
    }
    {
        auto document = pants::get(id(0));

        ASSERT_EQ("string", document["some"].get_utf8().value.to_string());
    }
}

/* Create a new document. */
TEST_F(model_db_test_fixture, new_document)
{
    pants document{};

    document.save();

    ASSERT_EQ(pants::count(), 331);
}

/* Test that deleting a document removes it from the database. */
TEST_F(model_db_test_fixture, get_removed_document_throws)
{
    pants document;

    document.save();

    const auto oid = document.oid();

    document.remove();

    ASSERT_THROW(pants::get(oid), ops::model_error);
}

/* Test that deleting a document leaves the object in an empty state. */
TEST_F(model_db_test_fixture, removed_document_is_empty)
{
    pants document;
    document.set_data("{\"some\":\"string\"}");

    document.save();
    document.remove();

    ASSERT_EQ(document.to_json().serialize(), "{}");
}

/* Test collection size. */
TEST_F(model_db_test_fixture, document_count_is_OK)
{
    ASSERT_EQ(pants::count(), 330);
}

/* Test default page size. */
TEST_F(model_db_test_fixture, default_page_size_is_OK)
{
    auto page = pants::page();
    const auto default_limit = pants::default_page_limit;

    ASSERT_EQ(page.count(), default_limit);
}

/* Test pagination page size. */
TEST_F(model_db_test_fixture, page_size_is_OK)
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

/* Test page element access. */
TEST_F(model_db_test_fixture, pagination_document_at)
{
    {
        auto page = pants::page(0, 4);

        ASSERT_EQ(page.at(0)["item"].get_int32().value, 0);
        ASSERT_EQ(page.at(3)["item"].get_int32().value, 3);
    }
    {
        auto page = pants::page(4, 8);

        ASSERT_EQ(page.at(0)["item"].get_int32().value, 4);
        ASSERT_EQ(page.at(3)["item"].get_int32().value, 7);
    }
}

/* Test that page element out of bounds access throws an exception. */
TEST_F(model_db_test_fixture, pagination_out_of_bounds)
{
    auto page = pants::page(0, 4);

    ASSERT_THROW(page.at(5)["item"], std::exception);
}

/* Test JSON serialization of page. */
TEST_F(model_db_test_fixture, pagination_to_json)
{
    auto collection = pants::page(0, 10);
    auto json = web::json::value::parse(collection.to_json().serialize());

    ASSERT_TRUE(json["count"].is_integer());
    ASSERT_TRUE(json["total"].is_integer());
    ASSERT_TRUE(json["pants"].is_array());

    ASSERT_EQ(json["count"].as_integer(), 10);
    ASSERT_EQ(json["total"].as_integer(), 330);
    ASSERT_EQ(json["pants"].as_array().size(), 10);
}

/* Test that prop type validation passes. */
TEST_F(model_db_test_fixture, prop_type_validation_no_throw)
{
    pants document{};

    auto json = document.to_json();
    json["name"] = web::json::value::string("test");
    document.set_data(json.serialize());

    ops::property_validator<pants> validator{}; 
    validator.add_property("name", ops::prop::t_string, true);

    ASSERT_NO_THROW(validator.validate(document));
}

/* Test that prop type validation passes. */
TEST_F(model_db_test_fixture, prop_type_validation_no_throw_2)
{
    pants document{};

    auto json = document.to_json();
    json["name"] = web::json::value::string("test");
    document.set_data(json.serialize());

    ops::property_validator<pants> validator{}; 
    validator.add_property(
        "name", 
        {ops::prop::t_string, ops::prop::t_numeric}, 
        true);

    ASSERT_NO_THROW(validator.validate(document));
}

/* Test that prop type validation passes. */
TEST_F(model_db_test_fixture, prop_type_validation_no_throw_3)
{
    pants document{};

    ops::property_validator<pants> validator{}; 
    validator.add_property("name", ops::prop::t_string);

    ASSERT_NO_THROW(validator.validate(document));
}

/* Test prop type validation failure. */
TEST_F(model_db_test_fixture, prop_type_validation_throw)
{
    pants document{};

    ops::property_validator<pants> validator{}; 
    validator.add_property("name", ops::prop::t_string, true);

    ASSERT_THROW(validator.validate(document), std::exception);
}

/* Test prop type validation failure. */
TEST_F(model_db_test_fixture, prop_type_validation_throw_2)
{
    pants document{};

    auto json = document.to_json();
    json["name"] = web::json::value::string("test");
    document.set_data(json.serialize());

    ops::property_validator<pants> validator{}; 
    validator.add_property(
        "name", 
        {ops::prop::t_boolean, ops::prop::t_numeric}, 
        true);

    ASSERT_THROW(validator.validate(document), std::exception);
}

/* Test prop type validation failure. */
TEST_F(model_db_test_fixture, prop_type_validation_throw_3)
{
    pants document{};

    auto json = document.to_json();
    json["name"] = web::json::value::string("test");
    document.set_data(json.serialize());

    ops::property_validator<pants> validator{}; 
    validator.add_property("name", ops::prop::t_null);

    ASSERT_THROW(validator.validate(document), std::exception);
}

/* Test uniqueness constraint validation success. */
TEST_F(model_db_test_fixture, unique_constraint_validation_no_throw)
{
    pants document{};

    web::json::value json{};
    json["name"] = web::json::value::string("test_1");
    document.set_data(json.serialize());

    ops::unique_constraint<pants> validator{}; 
    validator.add_key("name");

    ASSERT_NO_THROW(validator.validate(document));
}

/* Test uniqueness constraint validation failure. */
TEST_F(model_db_test_fixture, unique_constraint_validation_throw)
{
    {
        pants document{};

        web::json::value json{};
        json["name"] = web::json::value::string("test_1");
        document.set_data(json.serialize());

        ASSERT_NO_THROW(document.save());
    }
    {
        pants document{};

        web::json::value json{};
        json["name"] = web::json::value::string("test_1");
        document.set_data(json.serialize());

        ops::unique_constraint<pants> validator{}; 
        validator.add_key("name");

        ASSERT_THROW(validator.validate(document), std::exception);
    }
}

/* Test uniqueness constraint validation success when updating an existing document. */
TEST_F(model_db_test_fixture, unique_constraint_validation_update_no_throw)
{
    pants document{};

    {
        web::json::value json{};
        json["name"] = web::json::value::string("test_x");
        document.set_data(json.serialize());
    }

    document.save();

    pants other = pants::get(document.oid());
    other.set_data(document.data());

    ops::unique_constraint<pants> validator{}; 
    validator.add_key("name");

    ASSERT_NO_THROW(validator.validate(other));
}

int main(int argc, char* argv[])
{
    dotenv::init();
    mongocxx::instance instance{};

    ::testing::InitGoogleTest(&argc, argv);
    int result = RUN_ALL_TESTS();

    return result;
}
