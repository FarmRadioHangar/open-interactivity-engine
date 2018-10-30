#include "campaigns.h"
#include <nlohmann/json.hpp>
#include "../../ops/mongodb/counter.h"
#include "../../ops/mongodb/document.h"
#include "../../ops/mongodb/page.h"
#include "../../ops/util/json.h"
#include "../models/campaign.h"
#include "../models/language.h"

namespace core
{

using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::basic::make_document;
using web::http::methods;

campaigns_controller::campaigns_controller()
  : ops::http::rest::controller{}
{
}

void campaigns_controller::get_item(ops::http::request& request)
{
    const auto id = request.get_uri_param(1);
    const auto doc = ops::mongodb::document<campaign>::find("id", id);

    request.send_response({ {"campaign", ops::util::json::extract(doc)} });
}

void campaigns_controller::get(ops::http::request& request)
{
    const auto skip  = request.get_query_param<int64_t>("skip", 0);
    const auto limit = request.get_query_param<int64_t>("limit", 10);

    auto page = ops::mongodb::page<campaign>::get(skip, limit);

    auto j_campaigns = nlohmann::json::array();
    for (const auto& doc : page)
        j_campaigns.emplace_back(ops::util::json::extract(doc));

    request.send_response({ {"campaigns", j_campaigns} });
}

void campaigns_controller::post(ops::http::request& request)
{
    request.with_body([&request](const std::string& body)
    {
        auto j_campaign = nlohmann::json::parse(body);
        j_campaign["id"] = ops::mongodb::counter::generate_id();

        campaign model(j_campaign);

        ops::mongodb::document<campaign>::create(model.builder().extract());

        request.send_response({ {"campaign", j_campaign} });
    });
}

void campaigns_controller::post_feature(ops::http::request& request)
{
    request.with_body([&request](const std::string& body)
    {
        const auto id = request.get_uri_param(1);
        auto doc = ops::mongodb::document<campaign>::find("id", id);

        auto j_campaign = ops::util::json::extract(doc);
        auto j_feature = nlohmann::json::parse(body);

        const std::string feature_id = ops::mongodb::counter::generate_id();

        j_feature["id"] = feature_id;
        j_campaign["features"][feature_id] = j_feature;

        campaign model(j_campaign);

        doc.inject(model.builder().extract());
        doc.save();

        nlohmann::json res;
        res["campaign"] = j_campaign;
        res["feature"] = j_feature;

        request.send_response(res);
    });
}

void campaigns_controller::patch_feature(ops::http::request& request)
{
    request.with_body([&request](const std::string& body)
    {
        const auto campaign_id = request.get_uri_param(1);
        const auto feature_id  = request.get_uri_param(2);
        auto doc = ops::mongodb::document<campaign>::find("id", campaign_id);

        auto j_campaign = ops::util::json::extract(doc);
        auto j_request  = nlohmann::json::parse(body);
        auto& j_feature = j_campaign["features"][feature_id];

        j_feature.merge_patch(j_request);

        campaign model(j_campaign);

        doc.inject(model.builder().extract());
        doc.save();

        nlohmann::json res;
        res["campaign"] = j_campaign;
        res["feature"] = j_feature;
        res["feature"]["id"] = feature_id;

        request.send_response(res);
    });
}

void campaigns_controller::post_language(ops::http::request& request)
{
    request.with_body([&request](const std::string& body)
    {
        const auto id = request.get_uri_param(1);
        auto doc = ops::mongodb::document<campaign>::find("id", id);

        auto j_campaign = ops::util::json::extract(doc);
        auto j_request = nlohmann::json::parse(body);

        const std::string& tag = j_request["tag"];
        auto language_doc = ops::mongodb::document<language>::find("tag", tag);

        auto j_language = ops::util::json::extract(language_doc);

        j_campaign["languages"][tag] = j_language;

        campaign model(j_campaign);

        doc.inject(model.builder().extract());
        doc.save();

        request.send_response({
            {"campaign", j_campaign},
            {"language", j_language}
        });
    });
}

void campaigns_controller::post_adapter(ops::http::request& request)
{
    request.with_body([&request](const std::string& body)
    {
        const auto campaign_id = request.get_uri_param(1);
        const auto feature_id  = request.get_uri_param(2);
        auto doc = ops::mongodb::document<campaign>::find("id", campaign_id);

        auto j_campaign = ops::util::json::extract(doc);
        auto j_adapter = nlohmann::json::parse(body);

        const std::string& module = j_adapter["module"];

        j_campaign["features"][feature_id]["adapters"][module] = j_adapter;

        campaign model(j_campaign);

        doc.inject(model.builder().extract());
        doc.save();

        request.send_response({
            {"campaign", j_campaign},
            {"adapter", j_adapter}
        });
    });
}

void campaigns_controller::do_install(ops::http::rest::server* server)
{
    server->on(methods::POST, "^/campaigns/([0-9a-f]+)/features$",
        bind_handler<core::campaigns_controller>(&core::campaigns_controller::post_feature));

    server->on(methods::PATCH, "^/campaigns/([0-9a-f]+)/features/([0-9a-f]+)$",
        bind_handler<core::campaigns_controller>(&core::campaigns_controller::patch_feature));

    server->on(methods::POST, "^/campaigns/([0-9a-f]+)/features/([0-9a-f]+)/adapters$",
        bind_handler<core::campaigns_controller>(&core::campaigns_controller::post_adapter));

    server->on(methods::POST, "^/campaigns/([0-9a-f]+)/languages$",
        bind_handler<core::campaigns_controller>(&core::campaigns_controller::post_language));
}

} // namespace core
