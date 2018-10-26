#include "languages.h"
#include <nlohmann/json.hpp>
#include "../mongodb/counter.h"
#include "../mongodb/document.h"
#include "../mongodb/page.h"
#include "../builders/language.h"
#include "../util/json.h"

namespace ops
{

using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::basic::make_document;

languages_controller::languages_controller()
  : http::rest::controller{}
{
}

void languages_controller::get_item(http::request request)
{
    const auto id = request.get_uri_param(1);
    const auto doc = mongodb::document<languages>::find("id", id);

    request.send_response({ {"language", util::json::extract(doc)} });
}

void languages_controller::get(http::request request)
{
    const auto skip = request.get_query_param<int64_t>("skip", 0);
    const auto limit = request.get_query_param<int64_t>("limit", 10);

    auto page = ops::mongodb::page<languages>::get(skip, limit);

    auto items = nlohmann::json::array();
    for (const auto& doc : page)
        items.emplace_back(util::json::extract(doc));

    request.send_response({ {"languages", items} });
}

void languages_controller::post(http::request request)
{
    request.with_body([&request](const std::string& body)
    {
        auto j = nlohmann::json::parse(body);
        j["id"] = ops::mongodb::counter::generate_id();

        language_builder builder(j);

        mongodb::document<languages>::create(builder.extract());

        request.send_response({ {"language", j} });
    });
}

} // namespace ops
