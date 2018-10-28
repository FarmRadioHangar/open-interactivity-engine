#include "language.h"
#include <bsoncxx/builder/basic/kvp.hpp>

using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::basic::make_document;

namespace core
{

language::language(const nlohmann::json& j)
{
    _name = j.at("name");
    _tag = j.at("tag");

    if (j.end() != j.find("id")) {
        _id = j.at("id");
    }

//    append(kvp("name", std::string{j.at("name")}));
//    append(kvp("tag", std::string{j.at("tag")}));
//
//    if (j.end() != j.find("id")) {
//        append(kvp("id", std::string{j.at("id")}));
//    }
}

ops::mongodb::document<language> language::document() const
{
    bsoncxx::builder::basic::document builder{};

    builder.append(kvp("name", _name));
    builder.append(kvp("tag", _tag));

    //

    ops::mongodb::document<language> document{};
    document.inject(builder.extract());

    return document;
}

} // namespace core
