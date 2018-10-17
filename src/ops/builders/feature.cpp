#include "feature.h"
#include <bsoncxx/builder/basic/kvp.hpp>

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
}

} // namespace ops
