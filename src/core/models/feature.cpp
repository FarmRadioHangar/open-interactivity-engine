#include "feature.h"
#include <bsoncxx/builder/basic/array.hpp>
#include <bsoncxx/builder/basic/kvp.hpp>
#include <bsoncxx/json.hpp>
#include "adapter.h"

using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::basic::make_document;

namespace core
{

feature::feature(const nlohmann::json& j) : ops::mongodb::model<feature>{}
{
    _type = j.at("type");

    if (j.end() != j.find("id")) {
        _id = j.at("id");
    }

    if (j.end() != j.find("data")) {
        _data = j.at("data");
    }

    const auto& adapters = j.find("adapters");

    if (j.end() != adapters) {
        for (const nlohmann::json& j_adapter : *adapters) {
            _adapters.emplace_back(core::adapter{j_adapter});
        }
    }
}

bsoncxx::document::view feature::get_bson() const
{
    bsoncxx::builder::basic::document builder{};

    builder.append(kvp("type", _type));

    if (_id.has_value()) {
        builder.append(kvp("id", _id.value()));
    }

    if (_data.has_value()) {
        builder.append(kvp("data", bsoncxx::from_json(_data.value().dump())));
    }

    {
        bsoncxx::builder::basic::document collection_builder{};
        for (const auto& adapter : _adapters) {
            collection_builder.append(kvp(adapter.module(), adapter.bson()));
        }
        builder.append(kvp("features", collection_builder.extract()));
    }

    return builder.extract();
}

} // namespace core
