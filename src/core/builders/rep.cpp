#include "rep.h"
#include <bsoncxx/builder/basic/kvp.hpp>

using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::basic::make_document;

namespace core
{

rep_builder::rep_builder(const nlohmann::json& j)
  : ops::mongodb::bson::builder{j}
{
    append(kvp("format", std::string{j.at("format")}));
    append(kvp("language", std::string{j.at("language")}));

    if (j.end() != j.find("media")) {
        append(kvp("media", std::string{j.at("media")}));
    }
}

} // namespace core
