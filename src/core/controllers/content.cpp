#include "content.h"
#include <fstream>
#include <nlohmann/json.hpp>
#include "../../ops/mongodb/counter.h"
#include "../../ops/mongodb/document.h"
#include "../../ops/mongodb/page.h"
#include "../../ops/util/json.h"
#include "../models/content.h"
#include "../models/language.h"

namespace core
{

using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::basic::make_document;
using web::http::methods;

content_controller::content_controller()
  : ops::http::rest::controller{}
{
}

void content_controller::get_item(ops::http::request& request)
{
    const auto id = request.get_uri_param(1);
    const auto doc = ops::mongodb::document<content>::find("id", id);

    request.send_response({ {"content", ops::util::json::extract(doc)} });
}

void content_controller::get(ops::http::request& request)
{
    const auto skip = request.get_query_param<int64_t>("skip", 0);
    const auto limit = request.get_query_param<int64_t>("limit", 10);

    auto page = ops::mongodb::page<content>::get(skip, limit);

    auto j_content = nlohmann::json::array();
    for (const auto& doc : page)
        j_content.emplace_back(ops::util::json::extract(doc));

    request.send_response({ {"content", j_content} });
}

void content_controller::post(ops::http::request& request)
{
    request.with_body([&request](const std::string& body)
    {
        auto j_content = nlohmann::json::parse(body);
        j_content["id"] = ops::mongodb::counter::generate_id();

        content model(j_content);

        ops::mongodb::document<content>::create(model.bson());

        request.send_response({ {"content", j_content} });
    });
}

void content_controller::post_rep(ops::http::request& request)
{
    request.with_body([&request](const std::string& body)
    {
        const auto id = request.get_uri_param(1);
        auto doc = ops::mongodb::document<content>::find("id", id);

        auto j_content = ops::util::json::extract(doc);
        auto j_rep = nlohmann::json::parse(body);

        const std::string& tag = j_rep["language"];
        const std::string& format = j_rep["format"];

        // Check that language exists
        ops::mongodb::document<language>::find("tag", tag);

        j_content["reps"][format][tag] = j_rep;

        content model(j_content);

        doc.inject(model.bson());
        doc.save();

        request.send_response({ 
            {"content", j_content},
            {"rep", j_rep} 
        });
    });
}

void content_controller::do_install(ops::http::rest::server* server)
{
    server->on(methods::POST, "^/content/([0-9a-f]+)/reps$",
        bind_handler<core::content_controller>(&core::content_controller::post_rep));
}

} // namespace core
