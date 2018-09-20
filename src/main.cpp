/*!
 * \file main.cpp
 *
 * \mainpage Synopsis
 *
 * Welcome to the Open Survey Engine project
 */

#include <mongocxx/instance.hpp>
#include "controller.h"
#include "dotenv/dotenv.h"
#include "model.h"
#include "model/validator.h"
#include "model/validators/property_validator.h"
#include "model/validators/unique_constraint.h"
#include "server.h"

class campaigns : public ops::model<campaigns>
{
public:
    COLLECTION(campaigns)

    campaigns() : ops::model<campaigns>{"test"}
    {
        auto constraint = add_validator<ops::unique_constraint>();
        constraint->add_key("name");

        auto validator = add_validator<ops::property_validator>();
        validator->add_property("name", ops::prop::t_string, true);
    }
};

class languages : public ops::model<languages>
{
public:
    COLLECTION(languages)

    languages() : ops::model<languages>{"test"}
    {
        auto constraint = add_validator<ops::unique_constraint>();
        constraint->add_key("name");

        auto validator = add_validator<ops::property_validator>();
        validator->add_property("name", ops::prop::t_string, true);
        validator->add_property("tag", ops::prop::t_string, true);
    }
};

class audience : public ops::model<audience>
{
public:
    COLLECTION(audience)

    audience() : ops::model<audience>{"test"} {}
};

class content : public ops::model<content>
{
public:
    COLLECTION(content)

    content() : ops::model<content>{"test"} {}
};

class countries : public ops::model<countries>
{
public:
    COLLECTION(countries)

    countries() : ops::model<countries>{"test"} {}
};

using web::http::methods;

int main()
{
    dotenv::init();
    mongocxx::instance instance{};

    ops::http::server server{};

    ops::rest::controller<campaigns> campaigns_controller{&server};
    campaigns_controller.register_default_routes();

    ops::rest::controller<languages> languages_controller{&server};
    languages_controller.register_default_routes();

    ops::rest::controller<audience> audience_controller{&server};
    audience_controller.register_default_routes();

    ops::rest::controller<content> content_controller{&server};
    content_controller.register_default_routes();

    ops::rest::controller<countries> countries_controller{&server};
    countries_controller.register_default_routes();

    //
    {
        using bsoncxx::builder::basic::kvp;

        mongocxx::client client(mongocxx::uri{dotenv::getenv(
              "MONGODB_HOST",
              "mongodb://localhost:27017"
        )});

        auto db = client.database(dotenv::getenv("MONGODB_DATABASE", "test"));
        db.drop();

        auto collection = db.collection(campaigns::mongodb_collection);

        uint16_t i;
        for (i = 0; i < 330; i++)
        {
            bsoncxx::builder::basic::document builder{};
            builder.append(kvp("item", i));
            bsoncxx::document::value data = builder.extract();
            auto result = collection.insert_one(data.view());
            auto view = result.value().inserted_id().get_oid().value;
            std::cout << view.to_string() << std::endl;
        }
    }
    //

    server.run();
}
