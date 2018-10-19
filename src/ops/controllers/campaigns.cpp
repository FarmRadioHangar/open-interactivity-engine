#include "campaigns.h"
#include <nlohmann/json.hpp>
#include "../mongodb/counter.h"
#include "../mongodb/document.h"
#include "../mongodb/page.h"
#include "../builders/campaign.h"
#include "../builders/language.h"
#include "../util/json.h"

namespace ops
{

using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::basic::make_document;

campaigns_controller::campaigns_controller()
  : http::rest::controller{}
{
}

void campaigns_controller::get_item(http::request request)
{
    const auto id = request.get_uri_param(1);
    const auto doc = mongodb::document<campaigns>::find(make_document(kvp("id", id)));

    nlohmann::json res;
    res["campaign"] = util::json::builder(doc);

    request.send_response(res.dump());
}

void campaigns_controller::get(http::request request)
{
    const auto skip = request.get_query_param<int64_t>("skip", 0);
    const auto limit = request.get_query_param<int64_t>("limit", 10);

    auto page = ops::mongodb::page<campaigns>::get(skip, limit);

    auto items = nlohmann::json::array();
    for (const auto& doc : page)
        items.emplace_back(util::json::builder(doc));

    nlohmann::json res;
    res["campaigns"] = items;

    request.send_response(res.dump());
}

void campaigns_controller::post(http::request request)
{
    request.with_body([&request](const std::string& body)
    {
        auto j = nlohmann::json::parse(body);
        j["id"] = ops::mongodb::counter::generate_id();

        campaign_builder builder(j);

        mongodb::document<campaigns> doc{};
        doc.inject(builder.extract());
        doc.save();

        nlohmann::json res;
        res["campaign"] = j;

        request.send_response(res.dump());
    });
}

void campaigns_controller::post_feature(http::request request)
{
    request.with_body([&request](const std::string& body)
    {
        const auto id = request.get_uri_param(1);
        auto doc = mongodb::document<campaigns>::find(make_document(kvp("id", id)));

        auto j = util::json::builder(doc);

        auto j_feature = nlohmann::json::parse(body);
        const std::string feature_id = ops::mongodb::counter::generate_id();

        j["features"][feature_id] = j_feature;

        campaign_builder builder(j);

        doc.inject(builder.extract());
        doc.save();

        nlohmann::json res;
        res["campaign"] = j;
        res["feature"] = j_feature;
        res["feature"]["id"] = feature_id;

        request.send_response(res.dump());
    });
}

void campaigns_controller::patch_feature(http::request request)
{
    request.with_body([&request](const std::string& body)
    {
        const auto campaign_id = request.get_uri_param(1);
        const auto feature_id = request.get_uri_param(2);
        auto doc = mongodb::document<campaigns>::find(make_document(kvp("id", campaign_id)));

        auto j = util::json::builder(doc);
        auto j_request = nlohmann::json::parse(body);
        auto& j_feature = j["features"][feature_id];

        j_feature.merge_patch(j_feature);

        campaign_builder builder(j);

        doc.inject(builder.extract());
        doc.save();

        nlohmann::json res;
        res["campaign"] = j;
        res["feature"] = j_feature;
        res["feature"]["id"] = feature_id;

        request.send_response(res.dump());
    });
}

void campaigns_controller::post_language(http::request request)
{
    request.with_body([&request](const std::string& body)
    {
        const auto id = request.get_uri_param(1);
        auto doc = mongodb::document<campaigns>::find(make_document(kvp("id", id)));

        auto j = util::json::builder(doc);

        auto j_request = nlohmann::json::parse(body);

        const std::string& tag = j_request["tag"];
        auto language = mongodb::document<languages>::find(make_document(kvp("tag", tag)));

        auto j_language = util::json::builder(language);

        j["languages"][tag] = j_language;

        campaign_builder builder(j);

        doc.inject(builder.extract());
        doc.save();

        nlohmann::json res;
        res["campaign"] = j;
        res["language"] = j_language;

        request.send_response(res.dump());
    });
}

void campaigns_controller::post_adapter(http::request request)
{
    request.with_body([&request](const std::string& body)
    {
        const auto campaign_id = request.get_uri_param(1);
        const auto feature_id = request.get_uri_param(2);
        auto doc = mongodb::document<campaigns>::find(make_document(kvp("id", campaign_id)));

        auto j = util::json::builder(doc);
        auto j_adapter = nlohmann::json::parse(body);

        j["features"][feature_id]["adapters"].push_back(j_adapter);

        campaign_builder builder(j);

        doc.inject(builder.extract());
        doc.save();

        nlohmann::json res;
        res["campaign"] = j;
        res["adapter"] = j_adapter;

        request.send_response(res.dump());
    });
}

} // namespace ops
