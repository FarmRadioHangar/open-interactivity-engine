#include "session.h"
#include <bsoncxx/json.hpp>

using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::basic::make_document;

namespace twilio
{

session::session(const nlohmann::json& j)
  : ops::mongodb::model<session>{}
{
}

bsoncxx::builder::basic::document session::get_builder() const
{
    bsoncxx::builder::basic::document builder{};

    return builder;
}

}
