#include "country.h"
#include <bsoncxx/builder/basic/kvp.hpp>

using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::basic::make_document;

namespace ops
{

country_builder::country_builder(const nlohmann::json& j)
  : mongodb::bson::builder{j}
{
}

} // namespace ops
