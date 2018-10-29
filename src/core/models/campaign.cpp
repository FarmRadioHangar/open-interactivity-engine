#include "campaign.h"
#include <bsoncxx/builder/basic/kvp.hpp>
#include "feature.h"
#include "language.h"

using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::basic::make_document;

namespace core
{

campaign::campaign(const nlohmann::json& j)
  : ops::mongodb::model<campaign>{},
    _id{std::nullopt},
    _alias{std::nullopt}
{
    _name = j.at("name");

    if (j.end() != j.find("alias")) {
        _alias = j.at("alias");
    }

    if (j.end() != j.find("id")) {
        _id = j.at("id");
    }

    const auto& features = j.find("features");

    if (j.end() != features) {
        for (const nlohmann::json& j_feature: *features) {
            _features.emplace_back(core::feature{j_feature});
        }
    }

    const auto& languages = j.find("languages");

    if (j.end() != languages) {
        for (const nlohmann::json& j_language : *languages) {
            _languages.emplace_back(core::language{j_language});
        }
    }
}

bsoncxx::document::view campaign::get_bson() const
{
    bsoncxx::builder::basic::document builder{};

    builder.append(kvp("name", _name));

    if (_id.has_value()) {
        builder.append(kvp("id", _id.value()));
    }

    if (_alias.has_value()) {
        builder.append(kvp("alias", _alias.value()));
    }

    {
        bsoncxx::builder::basic::document collection_builder{};
        for (const auto& feature : _features) {
            collection_builder.append(kvp(feature.id().value(), feature.bson()));
        }
        builder.append(kvp("features", collection_builder.extract()));
    }

    {
        bsoncxx::builder::basic::document collection_builder{};
        for (const auto& language : _languages) {
            collection_builder.append(kvp(language.tag(), language.bson()));
        }
        builder.append(kvp("languages", collection_builder.extract()));
    }

    return builder.extract();
}

} // namespace core