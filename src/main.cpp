/*!
 * \file main.cpp
 *
 * \mainpage Synopsis
 *
 * Welcome to the Open Survey Engine project
 */

#include <mongocxx/instance.hpp>
#include "dotenv/dotenv.h"
#include "controller.h"
#include "model.h"
#include "server.h"
#include "model/validators/property_validator.h"
#include "model/validators/unique_constraint.h"
#include "model/validator.h"

class campaigns : public survey::model<campaigns>
{
public:
    COLLECTION(campaigns)

    campaigns() : survey::model<campaigns>{"test"}
    {
        auto constraint = add_validator<survey::unique_constraint>();
        constraint->add_key("name");

        auto validator = add_validator<survey::property_validator>();
        validator->add_property("name", survey::prop_type::t_string, true);
    }
};

class languages : public survey::model<languages>
{
public:
    COLLECTION(languages)

    languages() : survey::model<languages>{"test"} 
    {
        auto constraint = add_validator<survey::unique_constraint>();
        constraint->add_key("name");

        auto validator = add_validator<survey::property_validator>();
        validator->add_property("name", survey::prop_type::t_string, true);
        validator->add_property("tag", survey::prop_type::t_string, true);
    }
};

class audience : public survey::model<audience>
{
public:
    COLLECTION(audience)

    audience() : survey::model<audience>{"test"} {}
};

class content : public survey::model<content>
{
public:
    COLLECTION(content)

    content() : survey::model<content>{"test"} {}
};

using web::http::methods;

int main()
{
    dotenv::init();
    mongocxx::instance instance{};

    survey::http::server server{};

    survey::rest_controller<campaigns> campaigns_controller{&server};

    campaigns_controller.register_route(survey::route_type::t_get, 
        "^/campaigns$");

    campaigns_controller.register_route(survey::route_type::t_get_one, 
        "^/campaigns/([0-9a-f]+)$");

    campaigns_controller.register_route(survey::route_type::t_post, 
        "^/campaigns$");

    campaigns_controller.register_route(survey::route_type::t_put, 
        "^/campaigns/([0-9a-f]+)$");

    campaigns_controller.register_route(survey::route_type::t_delete, 
        "^/campaigns/([0-9a-f]+)$");
    
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
