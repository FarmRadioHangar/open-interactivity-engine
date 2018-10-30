#include "session.h"
#include <bsoncxx/json.hpp>

using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::basic::make_document;

namespace nexmo
{

session::session(const nlohmann::json& j)
  : ops::mongodb::model<session>{},
    _id{std::nullopt},
    _feature{std::nullopt}
{
    if (j.end() != j.find("id")) {
        _id = j.at("id");
    }

    _nexmo = j.at("conversation");

    const auto& j_campaign = j.at("campaign");
    _campaign_id = j_campaign.at("id");

    _feature = core::feature{j.at("feature")};
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

    builder.append(kvp("conversation", bsoncxx::from_json(_nexmo.dump())));

    builder.append(kvp("campaign", [this](bsoncxx::builder::basic::sub_document sub_builder) {
        sub_builder.append(kvp("id", _campaign_id));
    }));

    return builder;
}

} // namespace nexmo
