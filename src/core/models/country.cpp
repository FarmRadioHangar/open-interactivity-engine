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

bsoncxx::builder::basic::document country::get_builder() const
{
    bsoncxx::builder::basic::document builder{};

    return builder;
}

} // namespace core
