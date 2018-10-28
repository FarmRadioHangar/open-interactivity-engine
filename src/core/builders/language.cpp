#include "language.h"
#include <bsoncxx/builder/basic/kvp.hpp>

using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::basic::make_document;

namespace core
{

language::language(const nlohmann::json& j)
  : ops::mongodb::bson::builder{j}
{
//    append(kvp("name", std::string{j.at("name")}));
//    append(kvp("tag", std::string{j.at("tag")}));
//
//    if (j.end() != j.find("id")) {
//        append(kvp("id", std::string{j.at("id")}));
//    }
}

} // namespace core
