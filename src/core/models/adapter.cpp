#include "adapter.h"
#include <bsoncxx/builder/basic/kvp.hpp>
#include "../../ops/util/json.h"

using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::basic::make_document;

namespace core
{

adapter::adapter(const nlohmann::json& j)
  : ops::mongodb::model<adapter>{}
{
    _module = j.at("module");

    if (j.end() != j.find("data")) {
        _data = j.at("data");
    }
}

bsoncxx::builder::basic::document adapter::get_builder() const
{
    bsoncxx::builder::basic::document builder{};

    builder.append(kvp("module", _module));

    if (_data.has_value()) {
        builder.append(kvp("data", bsoncxx::from_json(_data.value().dump())));
    }

    return builder;
}

} // namespace core
