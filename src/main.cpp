/*!
 * \file main.cpp
 *
 * \mainpage Synopsis
 *
 * Welcome to the Open Survey Engine project
 */

#include <mongocxx/instance.hpp>
#include "dotenv/dotenv.h"
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

    languages() : survey::model<languages>{"test"} {}
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

using web::http::methods;

int main()
{
    //{
    //    auto val = web::json::value::parse("123.5");
    //    std::cout << val.is_number() << std::endl;
    //    return 0;
    //}

    dotenv::init();
    mongocxx::instance instance{};

    survey::http::server server{};

    server.on(methods::GET, "^/campaigns/([0-9a-f]+)$", campaigns_get_one);
    server.on(methods::POST, "^/campaigns$", campaigns_post);

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

//#include <bsoncxx/builder/stream/document.hpp>
//#include <bsoncxx/json.hpp>
//#include <cpprest/asyncrt_utils.h>
//#include <cpprest/http_listener.h>
//#include <iomanip>
//#include <iostream>
//#include <regex>
//#include <string>
//#include <thread>
//#include <vector>
//#include "model.h"
//#include "server.h"
//#include "builder/document.h"
//
//using namespace web;
//using namespace web::http;
//using namespace web::http::client;
//
//using bsoncxx::builder::basic::kvp;
//using bsoncxx::builder::basic::sub_document;
//using bsoncxx::builder::basic::sub_array;
//
//// TODO: https://github.com/Microsoft/cpprestsdk/wiki/FAQ#what-is-utilitystring_t-and-the-u-macro
//
//namespace polls
//{
//    class campaign : public polls::model<campaign>
//    {
//    public:
//        COLLECTION(campaigns)
//
//        campaign() : polls::model<campaign>{"test", campaign::mongodb_collection} {}
//    };
//
//    class language : public polls::model<language>
//    {
//    public:
//        COLLECTION(languages)
//
//        language() : polls::model<language>{"test", language::mongodb_collection} {}
//    };
//
//    class audience : public polls::model<audience>
//    {
//    public:
//        COLLECTION(audience)
//
//        audience() : polls::model<audience>{"test", audience::mongodb_collection} {}
//    };
//
//    class content : public polls::model<content>
//    {
//    public:
//        COLLECTION(content)
//
//        content() : polls::model<content>{"test", content::mongodb_collection} {}
//    };
//
//    class language_builder : public polls::utils::builder::document<language>
//    {
//    public:
//        explicit language_builder(web::json::value&& json);
//        language_builder(const std::string& oid, web::json::value&& json);
//
//    private:
//        void init();
//    };
//
//    language_builder::language_builder(web::json::value&& json)
//      : polls::utils::builder::document<language>{std::move(json)}
//    {
//        init();
//    }
//
//    language_builder::language_builder(const std::string& oid, web::json::value&& json)
//      : polls::utils::builder::document<language>{oid, std::move(json)}
//    {
//        init();
//    }
//
//    void language_builder::init()
//    {
//        add_property("name", json::value::value_type::String, true);
//        add_property("tag", json::value::value_type::String, true);
//
//        add_unique_constraint("name");
//    }
//}
//
//using namespace polls;
//
//void get_campaigns(polls::http::request request, polls::http::response response)
//{
//    auto skip = request.get_query_param<int64_t>("skip", 0);
//    auto limit = request.get_query_param<int64_t>("limit", 10);
//
//    response.set_body(campaign::all(skip, limit).to_json());
//    response.send();
//}
//
//void get_campaigns_item(polls::http::request request, polls::http::response response)
//{
//    auto document = campaign::get(request.get_uri_param(1));
//
//    json::value json_data{};
//    json_data["campaign"] = json::value::parse(document.data());
//
//    response.set_body(json_data);
//    response.send();
//}
//
//void delete_campaign(polls::http::request request, polls::http::response response)
//{
//    auto document = campaign::get(request.get_uri_param(1));
//    document.remove();
//    response.set_status_code(status_codes::NoContent);
//    response.send();
//}
//
//void post_campaign(polls::http::request request, polls::http::response response)
//{
//    request.with_json([&response](json::value data)
//    {
//        polls::utils::builder::document<campaign> builder(std::move(data));
//        builder.add_property("name", json::value::value_type::String, true);
//
//        auto document = builder.build();
//        document.save();
//
//        json::value json_data{};
//        json_data["campaign"] = json::value::parse(document.data());
//
//        response.set_body(json_data);
//        response.send();
//    });
//}
//
//void get_languages(polls::http::request request, polls::http::response response)
//{
//    auto skip = request.get_query_param<int64_t>("skip", 0);
//    auto limit = request.get_query_param<int64_t>("limit", 10);
//
//    response.set_body(language::all(skip, limit).to_json());
//    response.send();
//}
//
//void get_languages_item(polls::http::request request, polls::http::response response)
//{
//    auto document = language::get(request.get_uri_param(1));
//
//    json::value json_data{};
//    json_data["language"] = json::value::parse(document.data());
//
//    response.set_body(json_data);
//    response.send();
//}
//
//void delete_language(polls::http::request request, polls::http::response response)
//{
//    auto document = language::get(request.get_uri_param(1));
//    document.remove();
//
//    response.set_status_code(status_codes::NoContent);
//    response.send();
//}
//
//void put_language(polls::http::request request, polls::http::response response)
//{
//    request.with_json([&request, &response](json::value data)
//    {
//        auto oid = request.get_uri_param(1);
//        auto document = language_builder{oid, std::move(data)}.build();
//        document.save();
//
//        json::value json_response{};
//        json_response["language"] = json::value::parse(document.data());
//
//        response.set_body(json_response);
//        response.send();
//    });
//}
//
//void post_language(polls::http::request request, polls::http::response response)
//{
//    request.with_json([&response](json::value data)
//    {
//        auto document = language_builder{std::move(data)}.build();
//        document.save();
//
//        json::value json_response{};
//        json_response["language"] = json::value::parse(document.data());
//
//        response.set_body(json_response);
//        response.send();
//    });
//}
//
//void get_audience(polls::http::request request, polls::http::response response)
//{
//    auto skip = request.get_query_param<int64_t>("skip", 0);
//    auto limit = request.get_query_param<int64_t>("limit", 10);
//
//    response.set_body(audience::all(skip, limit).to_json());
//    response.send();
//}
//
//void get_audience_item(polls::http::request request, polls::http::response response)
//{
//    auto document = audience::get(request.get_uri_param(1));
//
//    json::value json_data{};
//    json_data["audience"] = json::value::parse(document.data());
//
//    response.set_body(json_data);
//    response.send();
//}
//
//void get_content(polls::http::request request, polls::http::response response)
//{
//    auto skip = request.get_query_param<int64_t>("skip", 0);
//    auto limit = request.get_query_param<int64_t>("limit", 10);
//
//    response.set_body(content::all(skip, limit).to_json());
//    response.send();
//}
//
//void get_content_item(polls::http::request request, polls::http::response response)
//{
//    auto document = content::get(request.get_uri_param(1));
//
//    json::value json_data{};
//    json_data["content"] = json::value::parse(document.data());
//
//    response.set_body(json_data);
//    response.send();
//}
//
//int main()
//{
//    mongocxx::instance instance{};
//
//    auto spec = bsoncxx::builder::stream::document{}
//        << "name" << 1
//        << bsoncxx::builder::stream::finalize;
//
//    mongocxx::options::index options{};
//    options.unique(true);
//
//    language::create_index(spec.view(), options);
//
//    polls::http::server server{};
//
//    server.on(web::http::methods::GET, "^/campaigns/([0-9a-f]+)$", get_campaigns_item);
//    server.on(web::http::methods::DEL, "^/campaigns/([0-9a-f]+)$", delete_campaign);
//    server.on(web::http::methods::GET, "^/campaigns$", get_campaigns);
//    server.on(web::http::methods::POST, "^/campaigns$", post_campaign);
//
//    server.on(web::http::methods::GET, "^/languages/([0-9a-f]+)$", get_languages_item);
//    server.on(web::http::methods::DEL, "^/languages/([0-9a-f]+)$", delete_language);
//    server.on(web::http::methods::PUT, "^/languages/([0-9a-f]+)$", put_language);
//    server.on(web::http::methods::GET, "^/languages$", get_languages);
//    server.on(web::http::methods::POST, "^/languages$", post_language);
//
//    server.on(web::http::methods::GET, "^/audience/([0-9a-f]+)$", get_audience_item);
//    server.on(web::http::methods::GET, "^/audience$", get_audience);
//
//    server.on(web::http::methods::GET, "^/content/([0-9a-f]+)$", get_content_item);
//    server.on(web::http::methods::GET, "^/content$", get_content);
//
//    server.run();
//
//    return 0;
//}
