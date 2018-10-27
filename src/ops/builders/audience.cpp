#include "audience.h"
#include <bsoncxx/builder/basic/kvp.hpp>

using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::basic::make_document;

namespace ops
{

audience_builder::audience_builder(const nlohmann::json& j)
  : mongodb::bson::builder{j}
{
}

} // namespace ops
