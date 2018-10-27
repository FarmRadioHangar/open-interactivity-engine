#include "feature.h"
#include <bsoncxx/builder/basic/array.hpp>
#include <bsoncxx/builder/basic/kvp.hpp>
#include <bsoncxx/json.hpp>
#include "adapter.h"

using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::basic::make_document;

namespace core
{

feature_builder::feature_builder(const nlohmann::json& j)
  : ops::mongodb::bson::builder{j}
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
        bsoncxx::builder::basic::document collection_builder{};
        for (nlohmann::json::const_iterator i = adapters->begin(); i != adapters->end(); ++i) {
            collection_builder.append(kvp(
                  std::string{i.key()}, 
                  adapter_builder{i.value()}.extract()));
        }
        append(kvp("adapters", collection_builder.extract()));
    }
}

} // namespace core
