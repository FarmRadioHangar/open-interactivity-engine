#include "country.h"
#include <bsoncxx/builder/basic/kvp.hpp>

using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::basic::make_document;

namespace core
{

country::country(const nlohmann::json& j)
  : ops::mongodb::model<country>{}
{
}

bsoncxx::document::view country::get_bson() const
{
    bsoncxx::builder::basic::document builder{};

    return builder.extract();
}

} // namespace core
