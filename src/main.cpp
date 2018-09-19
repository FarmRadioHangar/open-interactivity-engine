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

void campaigns_get(survey::http::request request)
{
    auto skip = request.get_query_param<int64_t>("skip", 0);
    auto limit = request.get_query_param<int64_t>("limit", 10);

    request.send_response(campaigns::page(skip, limit).to_json());
}

void campaigns_get_one(survey::http::request request)
{
    auto document = campaigns::get(request.get_uri_param(1));
    web::json::value response{};
    response["campaign"] = document.to_json();

    request.send_response(response);
}

void campaigns_post(survey::http::request request)
{
    request.with_body([&request](const std::string& body)
    {
        campaigns document{};
        
        document.set_data(body);
        document.validate();
        document.save();

        web::json::value response{};
        response["campaign"] = document.to_json();

        request.send_response(response);
    });
}

void campaigns_update(survey::http::request request)
{
    request.with_body([&request](const std::string& body)
    {
        auto document = campaigns::get(request.get_uri_param(1));
 
        document.set_data(body);
        document.validate();
        document.save();

        web::json::value response{};
        response["campaign"] = document.to_json();

        request.send_response(response);
    });
}

void campaigns_delete(survey::http::request request)
{
    auto document = campaigns::get(request.get_uri_param(1));
    document.remove();

    request.set_status_code(web::http::status_codes::NoContent);
    request.send_response();
}

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
