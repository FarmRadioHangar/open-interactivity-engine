#include "builder.h"

namespace ops
{
namespace mongodb
{
namespace bson
{

builder::builder(const nlohmann::json& j)
  : bsoncxx::builder::basic::document{}
{
}

bool builder::has_errors() const
{
    // todo
    return false;
}

} // namespace bson
} // namespace mongodb
} // namespace ops
