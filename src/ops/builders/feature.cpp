#include "feature.h"
#include <bsoncxx/builder/basic/array.hpp>
#include <bsoncxx/builder/basic/kvp.hpp>
#include <bsoncxx/json.hpp>
#include "adapter.h"

using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::basic::make_document;

namespace ops
{

feature_builder::feature_builder(const nlohmann::json& j)
  : mongodb::bson::builder{j}
{
    append(kvp("type", std::string{j.at("type")}));

    if (j.end() != j.find("id")) {
        append(kvp("id", std::string{j.at("id")}));
    }

    if (j.end() != j.find("data")) {
        append(kvp("data", bsoncxx::from_json(j.at("data").dump())));
    }

    const auto& adapters = j.find("adapters");

    if (j.end() != adapters) {
        bsoncxx::builder::basic::array array_builder{};
        for (const nlohmann::json& j_adapter : *adapters) {
            array_builder.append(adapter_builder{j_adapter}.extract());
        }
        append(kvp("adapters", array_builder.extract()));
    }
}

} // namespace ops
