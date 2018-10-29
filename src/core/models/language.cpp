#include "language.h"
#include <bsoncxx/builder/basic/kvp.hpp>

using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::basic::make_document;

namespace core
{

language::language(const nlohmann::json& j)
  : ops::mongodb::model<language>{},
    _id{std::nullopt}
{
    _name = j.at("name");
    _tag  = j.at("tag");

    if (j.end() != j.find("id")) {
        _id = j.at("id");
    } 
}

bsoncxx::document::view language::get_bson() const
{
    bsoncxx::builder::basic::document builder{};

    builder.append(kvp("name", _name));
    builder.append(kvp("tag", _tag));

    if (_id.has_value()) {
        builder.append(kvp("id", _id.value()));
    }

    return builder.extract();
}

} // namespace core
