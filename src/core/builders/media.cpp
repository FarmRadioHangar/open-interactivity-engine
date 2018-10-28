#include "media.h"
#include <bsoncxx/builder/basic/kvp.hpp>

using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::basic::make_document;

namespace core
{

media::media(const nlohmann::json& j)
  : ops::mongodb::bson::builder{j}
{
//    if (j.end() != j.find("id")) {
//        append(kvp("id", std::string{j.at("id")}));
//    }
//
//    if (j.end() != j.find("file")) {
//        append(kvp("file", std::string{j.at("file")}));
//    }
}

} // namespace core
