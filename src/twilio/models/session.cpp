#include "session.h"
#include <bsoncxx/json.hpp>

using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::basic::make_document;

namespace twilio
{

session::session(const nlohmann::json& j)
  : ops::mongodb::model<session>{}
{
    if (j.end() != j.find("id")) {
        _id = j.at("id");
    }
}

bsoncxx::builder::basic::document session::get_builder() const
{
    bsoncxx::builder::basic::document builder{};

    if (_id.has_value()) {
        builder.append(kvp("id", _id.value()));
    }

    return builder;
}

}
