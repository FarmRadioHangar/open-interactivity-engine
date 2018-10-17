#include "content.h"
#include <bsoncxx/builder/basic/kvp.hpp>

using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::basic::make_document;

namespace ops
{

content_builder::content_builder(const nlohmann::json& j)
  : mongodb::bson::builder{j}
{
    append(kvp("title", std::string{j.at("title")}));

    if (j.end() != j.find("id")) {
        append(kvp("id", std::string{j.at("id")}));
    }
}

} // namespace ops
