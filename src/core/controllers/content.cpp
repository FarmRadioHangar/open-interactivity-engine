#include "content.h"
#include <fstream>
#include <nlohmann/json.hpp>
#include "../../ops/mongodb/counter.h"
#include "../../ops/mongodb/document.h"
#include "../../ops/mongodb/page.h"
#include "../../ops/util/json.h"
#include "../builders/content.h"
#include "../builders/language.h"

namespace ops
{

using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::basic::make_document;
using web::http::methods;

content_controller::content_controller()
  : http::rest::controller{}
{
}

void content_controller::get_item(http::request& request)
{
    const auto id = request.get_uri_param(1);
    const auto doc = mongodb::document<content>::find("id", id);

    request.send_response({ {"content", util::json::extract(doc)} });
}

void content_controller::get(http::request& request)
{
    const auto skip = request.get_query_param<int64_t>("skip", 0);
    const auto limit = request.get_query_param<int64_t>("limit", 10);

    auto page = ops::mongodb::page<content>::get(skip, limit);

    auto j_content = nlohmann::json::array();
    for (const auto& doc : page)
        j_content.emplace_back(util::json::extract(doc));

    request.send_response({ {"content", j_content} });
}

void content_controller::post(http::request& request)
{
    request.with_body([&request](const std::string& body)
    {
        auto j_content = nlohmann::json::parse(body);
        j_content["id"] = ops::mongodb::counter::generate_id();

        content_builder builder(j_content);

        mongodb::document<content>::create(builder.extract());

        request.send_response({ {"content", j_content} });
    });
}

void content_controller::post_rep(http::request& request)
{
    request.with_body([&request](const std::string& body)
    {
        const auto id = request.get_uri_param(1);
        auto doc = mongodb::document<content>::find("id", id);

        auto j_content = util::json::extract(doc);
        auto j_rep = nlohmann::json::parse(body);

        const std::string& tag = j_rep["language"];
        const std::string& format = j_rep["format"];

        // Check that language exists
        mongodb::document<languages>::find("tag", tag);

        j_content["reps"][format][tag] = j_rep;

        content_builder builder(j_content);

        doc.inject(builder.extract());
        doc.save();

        request.send_response({ 
            {"content", j_content},
            {"rep", j_rep} 
        });
    });
}

void content_controller::do_install(http::rest::server* server)
{
    server->on(methods::POST, "^/content/([0-9a-f]+)/reps$",
        bind_handler<ops::content_controller>(&ops::content_controller::post_rep));
}

} // namespace ops
