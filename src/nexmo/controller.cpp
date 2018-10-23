#include "controller.h"
#include <bsoncxx/builder/basic/document.hpp>

namespace nexmo
{

using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::basic::make_document;
using web::http::methods;

controller::controller()
  : ops::http::rest::controller{}
{
}

void controller::test(ops::http::request request)
{
    //

    request.send_response();
}

void controller::do_install(ops::http::rest::server* server)
{
    server->add_route(methods::POST, "^/nexmo/test$",
        bind_handler<controller>(&controller::test));
}

} // namespace nexmo
