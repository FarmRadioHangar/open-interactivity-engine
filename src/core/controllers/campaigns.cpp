#include "campaigns.h"
#include <nlohmann/json.hpp>
#include "../../ops/mongodb/counter.h"
#include "../../ops/mongodb/document.h"
#include "../../ops/mongodb/page.h"
#include "../../ops/util/json.h"
#include "../builders/campaign.h"
#include "../builders/language.h"

namespace ops
{

using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::basic::make_document;
using web::http::methods;

campaigns_controller::campaigns_controller()
  : http::rest::controller{}
{
}

void campaigns_controller::get_item(http::request& request)
{
    const auto id = request.get_uri_param(1);
    const auto doc = mongodb::document<campaigns>::find("id", id);

    request.send_response({ {"campaign", util::json::extract(doc)} });
}

void campaigns_controller::get(http::request& request)
{
    const auto skip = request.get_query_param<int64_t>("skip", 0);
    const auto limit = request.get_query_param<int64_t>("limit", 10);

    auto page = ops::mongodb::page<campaigns>::get(skip, limit);

    auto j_campaigns = nlohmann::json::array();
    for (const auto& doc : page)
        j_campaigns.emplace_back(util::json::extract(doc));

    request.send_response({ {"campaigns", j_campaigns} });
}

void campaigns_controller::post(http::request& request)
{
    request.with_body([&request](const std::string& body)
    {
        auto j_campaign = nlohmann::json::parse(body);
        j_campaign["id"] = ops::mongodb::counter::generate_id();

        campaign_builder builder(j_campaign);

        mongodb::document<campaigns>::create(builder.extract());

        request.send_response({ {"campaign", j_campaign} });
    });
}

void campaigns_controller::post_feature(http::request& request)
{
    request.with_body([&request](const std::string& body)
    {
        const auto id = request.get_uri_param(1);
        auto doc = mongodb::document<campaigns>::find("id", id);

        auto j_campaign = util::json::extract(doc);
        auto j_feature = nlohmann::json::parse(body);

        const std::string feature_id = ops::mongodb::counter::generate_id();

        j_campaign["features"][feature_id] = j_feature;

        campaign_builder builder(j_campaign);

        doc.inject(builder.extract());
        doc.save();

        nlohmann::json res;
        res["campaign"] = j_campaign;
        res["feature"] = j_feature;
        res["feature"]["id"] = feature_id;

        request.send_response(res);
    });
}

void campaigns_controller::patch_feature(http::request& request)
{
    request.with_body([&request](const std::string& body)
    {
        const auto campaign_id = request.get_uri_param(1);
        const auto feature_id = request.get_uri_param(2);
        auto doc = mongodb::document<campaigns>::find("id", campaign_id);

        auto j_campaign = util::json::extract(doc);
        auto j_request = nlohmann::json::parse(body);
        auto& j_feature = j_campaign["features"][feature_id];

        j_feature.merge_patch(j_request);

        campaign_builder builder(j_campaign);

        doc.inject(builder.extract());
        doc.save();

        nlohmann::json res;
        res["campaign"] = j_campaign;
        res["feature"] = j_feature;
        res["feature"]["id"] = feature_id;

        request.send_response(res);
    });
}

void campaigns_controller::post_language(http::request& request)
{
    request.with_body([&request](const std::string& body)
    {
        const auto id = request.get_uri_param(1);
        auto doc = mongodb::document<campaigns>::find("id", id);

        auto j_campaign = util::json::extract(doc);
        auto j_request = nlohmann::json::parse(body);

        const std::string& tag = j_request["tag"];
        auto language = mongodb::document<languages>::find("tag", tag);

        auto j_language = util::json::extract(language);

        j_campaign["languages"][tag] = j_language;

        campaign_builder builder(j_campaign);

        doc.inject(builder.extract());
        doc.save();

        request.send_response({
            {"campaign", j_campaign},
            {"language", j_language}
        });
    });
}

void campaigns_controller::post_adapter(http::request& request)
{
    request.with_body([&request](const std::string& body)
    {
        const auto campaign_id = request.get_uri_param(1);
        const auto feature_id = request.get_uri_param(2);
        auto doc = mongodb::document<campaigns>::find("id", campaign_id);

        auto j_campaign = util::json::extract(doc);
        auto j_adapter = nlohmann::json::parse(body);

        const std::string& module = j_adapter["module"];

        j_campaign["features"][feature_id]["adapters"][module] = j_adapter;

        campaign_builder builder(j_campaign);

        doc.inject(builder.extract());
        doc.save();

        request.send_response({
            {"campaign", j_campaign},
            {"adapter", j_adapter}
        });
    });
}

void campaigns_controller::do_install(http::rest::server* server)
{
    server->on(methods::POST, "^/campaigns/([0-9a-f]+)/features$",
        bind_handler<ops::campaigns_controller>(&ops::campaigns_controller::post_feature));

    server->on(methods::PATCH, "^/campaigns/([0-9a-f]+)/features/([0-9a-f]+)$",
        bind_handler<ops::campaigns_controller>(&ops::campaigns_controller::patch_feature));

    server->on(methods::POST, "^/campaigns/([0-9a-f]+)/features/([0-9a-f]+)/adapters$",
        bind_handler<ops::campaigns_controller>(&ops::campaigns_controller::post_adapter));

    server->on(methods::POST, "^/campaigns/([0-9a-f]+)/languages$",
        bind_handler<ops::campaigns_controller>(&ops::campaigns_controller::post_language));
}

} // namespace ops
