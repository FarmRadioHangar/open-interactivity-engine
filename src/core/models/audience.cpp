#include "audience.h"
#include <bsoncxx/builder/basic/kvp.hpp>

using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::basic::make_document;

namespace core
{

audience::audience(const nlohmann::json& j)
  : ops::mongodb::model<audience>{}
{
}

bsoncxx::document::view audience::get_bson() const
{
    bsoncxx::builder::basic::document builder{};

    return builder.extract();
}

} // namespace core
