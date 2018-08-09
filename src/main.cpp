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
#include <thread>
#include "model.h"
#include "server.h"

template <typename T>
static std::string from_collection(const std::vector<T>& collection)
{
    std::stringstream oss{};
    oss << "{\"" << T::mongodb_collection << "\":[";
    for (auto i = collection.begin(); i != collection.end(); ++i) {
        if (i != collection.begin())
            oss << ",";
        oss << i->data();
    }
    oss << "]}";
    return oss.str();
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

using namespace polls;

void get_campaigns(web::http::http_request request, const std::smatch& match)
{
    request.reply(web::http::status_codes::OK, from_collection<campaign>(campaign::all()));
}

void get_campaigns_item(web::http::http_request request, const std::smatch& match)
{
    auto document = campaign::get(match.str(1));
    request.reply(web::http::status_codes::OK, document.data());
}

void get_languages(web::http::http_request request, const std::smatch& match)
{
    request.reply(web::http::status_codes::OK, from_collection<language>(language::all()));
}

void get_languages_item(web::http::http_request request, const std::smatch& match)
{
    auto document = language::get(match.str(1));
    request.reply(web::http::status_codes::OK, document.data());
}

void get_audience(web::http::http_request request, const std::smatch& match)
{
    request.reply(web::http::status_codes::OK, from_collection<audience>(audience::all()));
}

void get_audience_item(web::http::http_request request, const std::smatch& match)
{
    auto document = audience::get(match.str(1));
    request.reply(web::http::status_codes::OK, document.data());
}

void get_content(web::http::http_request request, const std::smatch& match)
{
    request.reply(web::http::status_codes::OK, from_collection<content>(content::all()));
}

void get_content_item(web::http::http_request request, const std::smatch& match)
{
    auto document = content::get(match.str(1));
    request.reply(web::http::status_codes::OK, document.data());
}

int main()
{
    mongocxx::instance instance{};

    polls::http::server server{};

    server.on(web::http::methods::GET, "^/campaigns/([0-9a-f]+)$", get_campaigns_item);
    server.on(web::http::methods::GET, "^/campaigns$", get_campaigns);

    server.on(web::http::methods::GET, "^/languages/([0-9a-f]+)$", get_languages_item);
    server.on(web::http::methods::GET, "^/languages$", get_languages);

    server.on(web::http::methods::GET, "^/audience/([0-9a-f]+)$", get_audience_item);
    server.on(web::http::methods::GET, "^/audience$", get_audience);

    server.on(web::http::methods::GET, "^/content/([0-9a-f]+)$", get_content_item);
    server.on(web::http::methods::GET, "^/content$", get_content);

    server.run();

    return 0;
}
