#include "rep.h"
#include <bsoncxx/builder/basic/kvp.hpp>

using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::basic::make_document;

namespace core
{

rep::rep(const nlohmann::json& j) 
  : ops::mongodb::model<rep>{},
    _media{std::nullopt}
{
    _format = j.at("format");
    _language = j.at("language");

    const auto& media = j.find("media");

    if (j.end() != media) {
        _media = core::media{j.at("media")};
    }
}

bsoncxx::document::view rep::get_bson() const
{
    bsoncxx::builder::basic::document builder{};

    builder.append(kvp("format", _format));
    builder.append(kvp("language", _language));

    if (_media.has_value()) {
        builder.append(kvp("media", _media.value().bson()));
    }

    return builder.extract();
}

} // namespace core
