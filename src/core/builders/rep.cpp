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

    const auto& media = j.find("media");

    if (j.end() != media) {
        bsoncxx::builder::basic::document media_builder{};
        media_builder.append(kvp("id", std::string{media->at("id")}));
        append(kvp("media", media_builder.extract()));
    }
}

} // namespace core
