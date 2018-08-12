/*!
 * \file main.cpp
 *
 * \mainpage Main
 */
#include <cpprest/asyncrt_utils.h>
#include <cpprest/http_listener.h>
#include <iostream>
#include <mongocxx/instance.hpp>
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

template <typename T, typename Collection = std::vector<T>>
json::value from_collection(const Collection& collection)
{
    std::vector<json::value> values;
    for (auto& value : collection) {
        values.push_back(json::value::parse(value.data()));
    }
    json::value obj;
    obj[T::mongodb_collection] = web::json::value::array(values);
    return obj;
}

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

template <typename T>
T type_conv(const std::string& str) { return T{str}; }

template <>
int64_t type_conv(const std::string& str) { return std::stoi(str); }

template <typename T>
int64_t get_param(const std::map<utility::string_t, utility::string_t>& params,
    const std::string& name, int64_t def)
{
    try {
        return type_conv<T>(params.at(name));
    } catch(std::exception&) {
        return def;
    }
}

using namespace polls;

void get_campaigns(web::http::http_request request, web::http::http_response response, const std::smatch& match)
{
    auto params = web::uri::split_query(request.request_uri().query());

    int64_t skip = get_param<int64_t>(params, "skip", 0);
    int64_t limit = get_param<int64_t>(params, "limit", 10);

    response.set_body(from_collection<campaign>(campaign::all(skip, limit)));
    request.reply(response);
}

void get_campaigns_item(web::http::http_request request, web::http::http_response response, const std::smatch& match)
{
    auto document = campaign::get(match.str(1));
    response.set_body(document.data());
    request.reply(response);
}

void post_campaign(web::http::http_request request, web::http::http_response response, const std::smatch& match)
{
    request
      .extract_json()
      .then([&request, &response](pplx::task<json::value> task)
      {
          polls::utils::builder::document<campaign> builder(task.get());
          builder.add_required_property("name", json::value::value_type::String);

          campaign document = builder.build();
          document.save();

          response.set_body(document.data());
          request.reply(response);
      })
      .wait();
}

void get_languages(web::http::http_request request, web::http::http_response response, const std::smatch& match)
{
    response.set_body(from_collection<language>(language::all()));
    request.reply(response);
}

void get_languages_item(web::http::http_request request, web::http::http_response response, const std::smatch& match)
{
    auto document = language::get(match.str(1));
    response.set_body(document.data());
    request.reply(response);
}

void post_language(web::http::http_request request, web::http::http_response response, const std::smatch& match)
{
    request
      .extract_json()
      .then([&request, &response](pplx::task<json::value> task)
      {
          polls::utils::builder::document<language> builder(task.get());
          builder.add_required_property("name", json::value::value_type::String);

          language document = builder.build();
          document.save();

          response.set_body(document.data());
          request.reply(response);
      })
      .wait();
}

void get_audience(web::http::http_request request, web::http::http_response response, const std::smatch& match)
{
    response.set_body(from_collection<audience>(audience::all()));
    request.reply(response);
}

void get_audience_item(web::http::http_request request, web::http::http_response response, const std::smatch& match)
{
    auto document = audience::get(match.str(1));
    response.set_body(document.data());
    request.reply(response);
}

void get_content(web::http::http_request request, web::http::http_response response, const std::smatch& match)
{
    response.set_body(from_collection<content>(content::all()));
    request.reply(response);
}

void get_content_item(web::http::http_request request, web::http::http_response response, const std::smatch& match)
{
    auto document = content::get(match.str(1));
    response.set_body(document.data());
    request.reply(response);
}

int main()
{
    mongocxx::instance instance{};

    polls::http::server server{};

    server.on(web::http::methods::GET, "^/campaigns/([0-9a-f]+)$", get_campaigns_item);
    server.on(web::http::methods::GET, "^/campaigns$", get_campaigns);
    server.on(web::http::methods::POST, "^/campaigns$", post_campaign);

    server.on(web::http::methods::GET, "^/languages/([0-9a-f]+)$", get_languages_item);
    server.on(web::http::methods::GET, "^/languages$", get_languages);
    server.on(web::http::methods::POST, "^/languages$", post_language);

    server.on(web::http::methods::GET, "^/audience/([0-9a-f]+)$", get_audience_item);
    server.on(web::http::methods::GET, "^/audience$", get_audience);

    server.on(web::http::methods::GET, "^/content/([0-9a-f]+)$", get_content_item);
    server.on(web::http::methods::GET, "^/content$", get_content);

    server.run();

    return 0;
}
