#include "content.h"
#include <fstream>
#include <nlohmann/json.hpp>
#include "../mongodb/counter.h"
#include "../mongodb/document.h"
#include "../mongodb/page.h"
#include "../builders/content.h"
#include "../builders/language.h"
#include "../builders/media.h"
#include "../util/json.h"

namespace ops
{

using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::basic::make_document;
using web::http::methods;

content_controller::content_controller()
  : http::rest::controller{}
{
}

void content_controller::get_item(http::request request)
{
    const auto id = request.get_uri_param(1);
    const auto doc = mongodb::document<content>::find("id", id);

    nlohmann::json res;
    res["content"] = util::json::extract(doc);

    request.send_response(res.dump());
}

void content_controller::get(http::request request)
{
    const auto skip = request.get_query_param<int64_t>("skip", 0);
    const auto limit = request.get_query_param<int64_t>("limit", 10);

    auto page = ops::mongodb::page<content>::get(skip, limit);

    auto items = nlohmann::json::array();
    for (const auto& doc : page)
        items.emplace_back(util::json::extract(doc));

    nlohmann::json res;
    res["content"] = items;

    request.send_response(res.dump());
}

void content_controller::post(http::request request)
{
    request.with_body([&request](const std::string& body)
    {
        auto j = nlohmann::json::parse(body);
        j["id"] = ops::mongodb::counter::generate_id();

        content_builder builder(j);

        mongodb::document<content> doc{};
        doc.inject(builder.extract());
        doc.save();

        nlohmann::json res;
        res["content"] = j;

        request.send_response(res.dump());
    });
}

void content_controller::post_rep(http::request request)
{
    request.with_body([&request](const std::string& body)
    {
        const auto id = request.get_uri_param(1);
        auto doc = mongodb::document<content>::find("id", id);

        auto j = util::json::extract(doc);

        auto j_rep = nlohmann::json::parse(body);

        const std::string& tag = j_rep["language"];
        const std::string& format = j_rep["format"];

        // Check that language exists
        mongodb::document<languages>::find("tag", tag);

        j["reps"][format][tag] = j_rep;

        content_builder builder(j);

        doc.inject(builder.extract());
        doc.save();

        nlohmann::json res;
        res["content"] = j;
        res["rep"] = j_rep;

        request.send_response(res.dump());
    });
}

void content_controller::post_media(http::request request)
{
    request.with_body([&request](const std::vector<unsigned char>& bytes)
    {
        nlohmann::json j_media;
        j_media["id"] = ops::mongodb::counter::generate_id();
        j_media["file"] = std::string{j_media["id"]} + ".mp3";

        std::ofstream outfile(j_media["file"], std::ios::out | std::ios::binary); 
        outfile.write(reinterpret_cast<const char*>(bytes.data()), bytes.size());

        media_builder builder(j_media);

        mongodb::document<media> doc{};
        doc.inject(builder.extract());
        doc.save();

        nlohmann::json res;
        res["media"] = j_media;

        request.send_response(res.dump());
    });
}

void content_controller::do_install(http::rest::server* server)
{
    server->on(methods::POST, "^/content/([0-9a-f]+)/reps$",
        bind_handler<ops::content_controller>(&ops::content_controller::post_rep));

    server->on(methods::POST, "^/media$",
        bind_handler<ops::content_controller>(&ops::content_controller::post_media));
}

} // namespace ops
