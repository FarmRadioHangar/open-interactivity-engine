#include "languages.h"
#include <nlohmann/json.hpp>
#include "../../ops/mongodb/counter.h"
#include "../../ops/mongodb/document.h"
#include "../../ops/mongodb/page.h"
#include "../../ops/util/json.h"
#include "../models/language.h"

namespace core
{

using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::basic::make_document;

languages_controller::languages_controller() : ops::http::rest::controller{}
{
}

void languages_controller::get_item(ops::http::request& request)
{
    const auto id = request.get_uri_param(1);
    const auto doc = ops::mongodb::document<language>::find("id", id);

    request.send_response({ {"language", ops::util::json::extract(doc)} });
}

void languages_controller::get(ops::http::request& request)
{
    const auto skip = request.get_query_param<int64_t>("skip", 0);
    const auto limit = request.get_query_param<int64_t>("limit", 10);

    auto page = ops::mongodb::page<language>::get(skip, limit);

    auto j_languages = nlohmann::json::array();
    for (const auto& doc : page)
        j_languages.emplace_back(ops::util::json::extract(doc));

    request.send_response({ {"languages", j_languages} });
}

void languages_controller::post(ops::http::request& request)
{
    request.with_body([&request](const std::string& body)
    {
        auto j_language = nlohmann::json::parse(body);
        j_language["id"] = ops::mongodb::counter::generate_id();

        language model(j_language);

        ops::mongodb::document<language>::create(model.bson());

        request.send_response({ {"language", j_language} });
    });
}

} // namespace core
