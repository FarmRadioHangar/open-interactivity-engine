#include "countries.h"
#include <nlohmann/json.hpp>
#include "../../ops/mongodb/counter.h"
#include "../../ops/mongodb/document.h"
#include "../../ops/mongodb/page.h"
#include "../../ops/util/json.h"
#include "../builders/country.h"

namespace core
{

using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::basic::make_document;

countries_controller::countries_controller()
  : ops::http::rest::controller{}
{
}

} // namespace core
