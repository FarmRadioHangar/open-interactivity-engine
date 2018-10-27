#include "session_builder.h"
#include <bsoncxx/json.hpp>
#include "../../core/builders/feature.h"

using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::basic::make_document;

namespace nexmo
{

session_builder::session_builder(const nlohmann::json& j)
  : ops::mongodb::bson::builder{j}
{
    nlohmann::json j_campaign = j.at("campaign");

    bsoncxx::builder::basic::document campaign_builder{};
    campaign_builder.append(kvp("id", std::string{j_campaign.at("id")}));

    append(kvp("campaign", campaign_builder.extract()));

    append(kvp("nexmo", bsoncxx::from_json(j.at("nexmo").dump())));

    if (j.end() != j.find("id")) {
        append(kvp("id", std::string{j.at("id")}));
    }

    core::feature_builder builder{j.at("feature")};

    append(kvp("feature", builder.extract()));
}

} // namespace nexmo
