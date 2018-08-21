/*!
 * \file main.cpp
 *
 * \mainpage Main
 */
#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/json.hpp>
#include <cpprest/asyncrt_utils.h>
#include <cpprest/http_listener.h>
#include <iomanip>
#include <iostream>
#include <regex>
#include <string>
#include <thread>
#include <vector>
#include "model.h"
#include "server.h"
#include "utils.h"

using namespace web;
using namespace web::http;
using namespace web::http::client;

using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::basic::sub_document;
using bsoncxx::builder::basic::sub_array;

// TODO: https://github.com/Microsoft/cpprestsdk/wiki/FAQ#what-is-utilitystring_t-and-the-u-macro

namespace polls
{
    class campaign : public polls::model<campaign>
    {
    public:
        COLLECTION(campaigns)

        campaign() : polls::model<campaign>{"test", campaign::mongodb_collection} {}
    };

    class language : public polls::model<language>
    {
    public:
        COLLECTION(languages)

        language() : polls::model<language>{"test", language::mongodb_collection} {}
    };

    class audience : public polls::model<audience>
    {
    public:
        COLLECTION(audience)

        audience() : polls::model<audience>{"test", audience::mongodb_collection} {}
    };

    class content : public polls::model<content>
    {
    public:
        COLLECTION(content)

        content() : polls::model<content>{"test", content::mongodb_collection} {}
    };
}

using namespace polls;

void get_campaigns(polls::http::request request, polls::http::response response)
{
    auto skip = request.get_query_param<int64_t>("skip", 0);
    auto limit = request.get_query_param<int64_t>("limit", 10);

    response.set_body(campaign::all(skip, limit).json());
    response.send();
}

void get_campaigns_item(polls::http::request request, polls::http::response response)
{
    auto document = campaign::get(request.get_uri_param(1));

    json::value json_data{};
    json_data["campaign"] = json::value::parse(document.data());

    response.set_body(json_data);
    response.send();
}

void delete_campaign(polls::http::request request, polls::http::response response)
{
    auto document = campaign::get(request.get_uri_param(1));
    document.remove();
    response.set_status_code(status_codes::NoContent);
    response.send();
}

void post_campaign(polls::http::request request, polls::http::response response)
{
    request.with_json([&response](json::value data)
    {
        polls::utils::builder::document<campaign> builder(data);
        builder.add_required_property("name", json::value::value_type::String);

        auto document = builder.build();
        document.save();

        json::value json_data{};
        json_data["campaign"] = json::value::parse(document.data());

        response.set_body(json_data);
        response.send();
    });
}

void get_languages(polls::http::request request, polls::http::response response)
{
    auto skip = request.get_query_param<int64_t>("skip", 0);
    auto limit = request.get_query_param<int64_t>("limit", 10);

    response.set_body(language::all(skip, limit).json());
    response.send();
}

void get_languages_item(polls::http::request request, polls::http::response response)
{
    auto document = language::get(request.get_uri_param(1));

    json::value json_data{};
    json_data["language"] = json::value::parse(document.data());

    response.set_body(json_data);
    response.send();
}

void delete_language(polls::http::request request, polls::http::response response)
{
    auto document = language::get(request.get_uri_param(1));
    document.remove();
    response.set_status_code(status_codes::NoContent);
    response.send();
}

void post_language(polls::http::request request, polls::http::response response)
{
    request.with_json([&response](json::value data)
    {
        polls::utils::builder::document<language> builder(data);
        builder.add_required_property("name", json::value::value_type::String);
        builder.add_required_property("iso_code", json::value::value_type::String);

        auto document = builder.build();

        auto filter = bsoncxx::builder::basic::make_document(
            kvp("name", data["name"].as_string())
        );

        if (language::count(filter.view()) > 0) {
            json::value json_data{};
            json_data["error"]  = json::value::string("duplicate key");
            json_data["status"] = json::value::number(409);
            json_data["key"]    = json::value::string("name");
            json_data["value"]  = json::value::string(data["name"].as_string());
            response.set_status_code(409);
            response.set_body(json_data);
            response.send();
            return;
        }

        document.save();

        json::value json_data{};
        json_data["language"] = json::value::parse(document.data());

        response.set_body(json_data);
        response.send();
    });
}

void get_audience(polls::http::request request, polls::http::response response)
{
    auto skip = request.get_query_param<int64_t>("skip", 0);
    auto limit = request.get_query_param<int64_t>("limit", 10);

    response.set_body(audience::all(skip, limit).json());
    response.send();
}

void get_audience_item(polls::http::request request, polls::http::response response)
{
    auto document = audience::get(request.get_uri_param(1));

    json::value json_data{};
    json_data["audience"] = json::value::parse(document.data());

    response.set_body(json_data);
    response.send();
}

void get_content(polls::http::request request, polls::http::response response)
{
    auto skip = request.get_query_param<int64_t>("skip", 0);
    auto limit = request.get_query_param<int64_t>("limit", 10);

    response.set_body(content::all(skip, limit).json());
    response.send();
}

void get_content_item(polls::http::request request, polls::http::response response)
{
    auto document = content::get(request.get_uri_param(1));

    json::value json_data{};
    json_data["content"] = json::value::parse(document.data());

    response.set_body(json_data);
    response.send();
}

int main()
{
    mongocxx::instance instance{};

    auto spec = bsoncxx::builder::stream::document{}
        << "name" << 1
        << bsoncxx::builder::stream::finalize;

    mongocxx::options::index options{};
    options.unique(true);

    language::create_index(spec.view(), options);

    polls::http::server server{};

    server.on(web::http::methods::GET, "^/campaigns/([0-9a-f]+)$", get_campaigns_item);
    server.on(web::http::methods::DEL, "^/campaigns/([0-9a-f]+)$", delete_campaign);
    server.on(web::http::methods::GET, "^/campaigns$", get_campaigns);
    server.on(web::http::methods::POST, "^/campaigns$", post_campaign);

    server.on(web::http::methods::GET, "^/languages/([0-9a-f]+)$", get_languages_item);
    server.on(web::http::methods::DEL, "^/languages/([0-9a-f]+)$", delete_language);
    server.on(web::http::methods::GET, "^/languages$", get_languages);
    server.on(web::http::methods::POST, "^/languages$", post_language);

    server.on(web::http::methods::GET, "^/audience/([0-9a-f]+)$", get_audience_item);
    server.on(web::http::methods::GET, "^/audience$", get_audience);

    server.on(web::http::methods::GET, "^/content/([0-9a-f]+)$", get_content_item);
    server.on(web::http::methods::GET, "^/content$", get_content);

    server.run();

    return 0;
}
