#include "media.h"
#include <bsoncxx/builder/basic/kvp.hpp>

using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::basic::make_document;

namespace ops
{

media_builder::media_builder(const nlohmann::json& j)
  : mongodb::bson::builder{j}
{
    if (j.end() != j.find("id")) {
        append(kvp("id", std::string{j.at("id")}));
    }
}

} // namespace ops
