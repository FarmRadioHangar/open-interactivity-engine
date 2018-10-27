#include "campaign.h"
#include <bsoncxx/builder/basic/kvp.hpp>
#include "feature.h"
#include "language.h"

using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::basic::make_document;

namespace core
{

campaign_builder::campaign_builder(const nlohmann::json& j)
  : ops::mongodb::bson::builder{j}
{
    append(kvp("name", std::string{j.at("name")}));

    if (j.end() != j.find("alias")) {
        append(kvp("alias", std::string{j.at("alias")}));
    }

    if (j.end() != j.find("id")) {
        append(kvp("id", std::string{j.at("id")}));
    }

    const auto& features = j.find("features");

    if (j.end() != features) {
        bsoncxx::builder::basic::document collection_builder{};
        for (nlohmann::json::const_iterator i = features->begin(); i != features->end(); ++i) {
            collection_builder.append(kvp(
                  std::string{i.key()}, 
                  feature_builder{i.value()}.extract()));
        }
        append(kvp("features", collection_builder.extract()));
    }

    const auto& languages = j.find("languages");

    if (j.end() != languages) {
        bsoncxx::builder::basic::document collection_builder{};
        for (const nlohmann::json& j_language : *languages) {
            collection_builder.append(kvp(
                  std::string{j_language["tag"]}, 
                  language_builder{j_language}.extract()));
        }
        append(kvp("languages", collection_builder.extract()));
    }
}

} // namespace core
