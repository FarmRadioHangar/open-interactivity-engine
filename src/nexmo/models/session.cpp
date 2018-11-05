#include "session.h"
#include <bsoncxx/json.hpp>

using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::basic::make_document;

namespace nexmo
{

session::session(const nlohmann::json& j)
  : ops::mongodb::model<session>{},
    _id{std::nullopt},
    _feature{std::nullopt},
    _campaign_id{std::nullopt},
    _conversation{},
    _events{nlohmann::json::array()}
{
    if (j.end() != j.find("id")) {
        _id = j.at("id");
    }

    if (j.end() != j.find("conversation")) {
        _conversation = j.at("conversation");
    }

    if (j.end() != j.find("events")) {
        _events = j.at("events");
    }

    if (j.end() != j.find("campaign")) {
        const auto& j_campaign = j.at("campaign");
        _campaign_id = j_campaign.at("id");
    }

    if (j.end() != j.find("feature")) {
        _feature = core::feature{j.at("feature")};
    }
}

bsoncxx::builder::basic::document session::get_builder() const
{
    bsoncxx::builder::basic::document builder{};

    if (_id.has_value()) {
        builder.append(kvp("id", _id.value()));
    }

    if (_feature.has_value()) {
        builder.append(kvp("feature", _feature.value().builder().extract()));
    }

    builder.append(kvp("conversation", bsoncxx::from_json(_conversation.dump())));

    if (_events.is_array()) {
        bsoncxx::builder::basic::array array_builder{};
        for (const auto& j_event : _events) {
            array_builder.append(bsoncxx::from_json(j_event.dump()));
        }
        builder.append(kvp("events", array_builder.extract()));
    }

    if (_campaign_id.has_value()) {
        builder.append(kvp("campaign", [this](bsoncxx::builder::basic::sub_document sub_builder) {
            sub_builder.append(kvp("id", _campaign_id.value()));
        }));
    }

    return builder;
}

} // namespace nexmo
