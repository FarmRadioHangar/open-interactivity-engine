#include "controller.h"
#include <bsoncxx/builder/basic/document.hpp>

namespace nexmo
{

using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::basic::make_document;

controller::controller()
  : ops::http::rest::controller{}
{
}

} // namespace nexmo
