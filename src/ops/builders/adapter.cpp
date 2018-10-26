#include "adapter.h"
#include <bsoncxx/builder/basic/kvp.hpp>
#include "../util/json.h"

using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::basic::make_document;

namespace ops
{

adapter_builder::adapter_builder(const nlohmann::json& j)
  : mongodb::bson::builder{j}
{
    append(kvp("module", std::string{j.at("module")}));

    if (j.end() != j.find("data")) {
        append(kvp("data", bsoncxx::from_json(j.at("data").dump())));
    }
}

} // namespace ops
