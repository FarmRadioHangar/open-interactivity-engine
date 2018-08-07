/*!
 * \file main.cpp
 *
 * \mainpage Main
 */
#include <iostream>
#include <mongocxx/instance.hpp>
#include <thread>
#include "model.h"
#include "server.h"

//template <typename T>
//static std::string from_collection(const std::vector<T>& collection)
//{
//    std::stringstream oss{};
//    oss << "{\"" << T::mongodb_collection << "\":[";
//    for (auto i = collection.begin(); i != collection.end(); ++i) {
//        if (i != collection.begin())
//            oss << ",";
//        oss << i->data();
//    }
//    oss << "]}";
//    return oss.str();
//}
//
//class campaign : public polls::model<campaign>
//{
//public:
//    COLLECTION(campaigns)
//
//    campaign() : polls::model<campaign>{"uliza", campaign::mongodb_collection} {}
//};
//
//class language : public polls::model<language>
//{
//public:
//    COLLECTION(languages)
//
//    language() : polls::model<language>{"uliza", language::mongodb_collection} {}
//};
//
//class audience : public polls::model<audience>
//{
//public:
//    COLLECTION(audience)
//
//    audience() : polls::model<audience>{"uliza", audience::mongodb_collection} {}
//};
//
//class content : public polls::model<content>
//{
//public:
//    COLLECTION(content)
//
//    content() : polls::model<content>{"uliza", content::mongodb_collection} {}
//};
//
//void get_campaigns(polls::http::request req, polls::http::response res)
//{
//    res.send_json(polls::http::status_code::success_ok, "{}");
//    //res.send_json(
//    //    polls::http::status_code::success_ok,
//    //    from_collection<campaign>(campaign::all()));
//}
//
//void get_campaigns_item(polls::http::request req, polls::http::response res)
//{
//    auto document = campaign::get(req.param(1));
//    res.send_json(polls::http::status_code::success_ok, document.data());
//}
//
//void get_languages(polls::http::request req, polls::http::response res)
//{
//    res.send_json(
//        polls::http::status_code::success_ok,
//        from_collection<language>(language::all()));
//}
//
//void get_languages_item(polls::http::request req, polls::http::response res)
//{
//    auto document = language::get(req.param(1));
//    res.send_json(polls::http::status_code::success_ok, document.data());
//}
//
//void get_audience(polls::http::request req, polls::http::response res)
//{
//    res.send_json(
//        polls::http::status_code::success_ok,
//        from_collection<audience>(audience::all()));
//}
//
//void get_audience_item(polls::http::request req, polls::http::response res)
//{
//    auto document = audience::get(req.param(1));
//    res.send_json(polls::http::status_code::success_ok, document.data());
//}
//
//void get_content(polls::http::request req, polls::http::response res)
//{
//    res.send_json(
//        polls::http::status_code::success_ok,
//        from_collection<content>(content::all()));
//}
//
//void get_content_item(polls::http::request req, polls::http::response res)
//{
//    auto document = content::get(req.param(1));
//    res.send_json(polls::http::status_code::success_ok, document.data());
//}

int main()
{
    mongocxx::instance instance{};

    polls::http::server server{};

    //server.on(polls::http::GET, "^/campaigns/([0-9a-f]+)$", get_campaigns_item);
    //server.on(polls::http::GET, "^/campaigns$", get_campaigns);

    //server.on(polls::http::GET, "^/languages/([0-9a-f]+)$", get_languages_item);
    //server.on(polls::http::GET, "^/languages$", get_languages);

    //server.on(polls::http::GET, "^/audience/([0-9a-f]+)$", get_audience_item);
    //server.on(polls::http::GET, "^/audience$", get_audience);

    //server.on(polls::http::GET, "^/content/([0-9a-f]+)$", get_content_item);
    //server.on(polls::http::GET, "^/content$", get_content);

    server.run();

    return 0;
}
