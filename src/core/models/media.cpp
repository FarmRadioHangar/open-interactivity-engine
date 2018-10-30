#include "media.h"
#include <bsoncxx/builder/basic/kvp.hpp>

using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::basic::make_document;

namespace core
{

media::media(const nlohmann::json& j)
  : ops::mongodb::model<media>{},
    _id{std::nullopt},
    _file{std::nullopt}
{
    if (j.end() != j.find("id")) {
        _id = j.at("id");
    }

    if (j.end() != j.find("file")) {
        _file = j.at("file");
    }
}

bsoncxx::builder::basic::document media::get_builder() const
{
    bsoncxx::builder::basic::document builder{};

    if (_id.has_value()) {
        builder.append(kvp("id", _id.value()));
    }

    if (_file.has_value()) {
        builder.append(kvp("file", _file.value()));
    }

    return builder;
}

} // namespace core
