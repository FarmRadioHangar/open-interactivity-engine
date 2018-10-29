#include "audience.h"
#include <nlohmann/json.hpp>
#include "../../ops/mongodb/counter.h"
#include "../../ops/mongodb/document.h"
#include "../../ops/mongodb/page.h"
#include "../../ops/util/json.h"
#include "../models/audience.h"

namespace core
{

using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::basic::make_document;

audience_controller::audience_controller()
  : ops::http::rest::controller{}
{
}

} // namespace core
