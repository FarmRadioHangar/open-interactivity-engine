#include <iostream>
#include <mongocxx/instance.hpp>
#include <sstream>
#include <thread>
#include "model.h"
#include "server.h"

class campaign : public polls::model<campaign>
{
public:
    static constexpr auto mongodb_collection = "campaigns";

    campaign() : polls::model<campaign>{"uliza", campaign::mongodb_collection} {}
};

class language : public polls::model<language>
{
public:
    static constexpr auto mongodb_collection = "languages";

    language() : polls::model<language>{"uliza", language::mongodb_collection} {}
};

class audience : public polls::model<audience>
{
public:
    static constexpr auto mongodb_collection = "audience";

    audience() : polls::model<audience>{"uliza", audience::mongodb_collection} {}
};

class content : public polls::model<content>
{
public:
    static constexpr auto mongodb_collection = "content";

    content() : polls::model<content>{"uliza", content::mongodb_collection} {}
};

void get_campaigns(polls::http::request req, polls::http::response res)
{
    auto collection = campaign::all();

    std::ostringstream oss{};
    oss << "{\"campaigns\":[";
    for (auto i = collection.begin(); i != collection.end(); ++i) {
        if (i != collection.begin()) 
            oss << ",";
        oss << i->data();
    }
    oss << "]}";

    res.send_json(polls::http::status_code::success_ok, oss.str());
}

void get_campaign(polls::http::request req, polls::http::response res)
{
    auto document = campaign::get(req.param(1));
    res.send_json(polls::http::status_code::success_ok, document.data());
}

int main()
{
    mongocxx::instance instance{};

    polls::http::server server{};

    server.on(polls::http::GET, "^/campaigns/([0-9a-f]+)$", get_campaign);
    server.on(polls::http::GET, "^/campaigns$", get_campaigns);

    server.run();

    return 0;
}
