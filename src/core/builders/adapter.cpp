#include "adapter.h"
#include <bsoncxx/builder/basic/kvp.hpp>
#include "../../ops/util/json.h"

using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::basic::make_document;

namespace core
{

adapter::adapter(const nlohmann::json& j)
  : ops::mongodb::bson::builder{j}
{
    _module = j.at("module");
    _data = j.at("data");

    //append(kvp("module", std::string{j.at("module")}));

    //if (j.end() != j.find("data")) {
    //    append(kvp("data", bsoncxx::from_json(j.at("data").dump())));
    //}
}

} // namespace core
