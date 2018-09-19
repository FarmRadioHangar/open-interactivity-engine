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

class campaigns : public otp::model<campaigns>
{
public:
    COLLECTION(campaigns)

    campaigns() : otp::model<campaigns>{"test"}
    {
        auto constraint = add_validator<otp::unique_constraint>();
        constraint->add_key("name");

        auto validator = add_validator<otp::property_validator>();
        validator->add_property("name", otp::prop_type::t_string, true);
    }
};

class languages : public otp::model<languages>
{
public:
    COLLECTION(languages)

    languages() : otp::model<languages>{"test"} 
    {
        auto constraint = add_validator<otp::unique_constraint>();
        constraint->add_key("name");

        auto validator = add_validator<otp::property_validator>();
        validator->add_property("name", otp::prop_type::t_string, true);
        validator->add_property("tag", otp::prop_type::t_string, true);
    }
};

class audience : public otp::model<audience>
{
public:
    COLLECTION(audience)

    audience() : otp::model<audience>{"test"} {}
};

class content : public otp::model<content>
{
public:
    COLLECTION(content)

    content() : otp::model<content>{"test"} {}
};

using web::http::methods;

int main()
{
    dotenv::init();
    mongocxx::instance instance{};

    otp::http::server server{};

    otp::rest_controller<campaigns> campaigns_controller{&server};

    campaigns_controller.register_route(otp::route_type::t_get, 
        "^/campaigns$");

    campaigns_controller.register_route(otp::route_type::t_get_one, 
        "^/campaigns/([0-9a-f]+)$");

    campaigns_controller.register_route(otp::route_type::t_post, 
        "^/campaigns$");

    campaigns_controller.register_route(otp::route_type::t_put, 
        "^/campaigns/([0-9a-f]+)$");

    campaigns_controller.register_route(otp::route_type::t_delete, 
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
