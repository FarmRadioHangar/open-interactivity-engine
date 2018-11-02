#include "twilio_controller.h"
#include <bsoncxx/builder/basic/document.hpp>

namespace twilio
{

using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::basic::make_document;
using web::http::methods;

controller::controller()
  : ops::http::rest::controller{}
{
}

void controller::do_install(ops::http::rest::server* server)
{
//    server->on(methods::POST, "^/twilio/ivr/s/([0-9a-f]+)/n/([0-9]+)$",
//        bind_handler<controller>(&controller::post_ivr));
//
//    server->on(methods::POST, "^/twilio/event$",
//        bind_handler<controller>(&controller::post_event));
//
//    server->on(methods::POST, "^/twilio/answer/c/([0-9a-f]+)/f/([0-9a-f]+)$",
//        bind_handler<controller>(&controller::post_answer));
}

} // namespace twilio
