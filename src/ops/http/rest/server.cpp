#include "server.h"
#include <cassert>
#include "controller.h"

namespace ops
{
namespace http
{
namespace rest
{

server::server() : http::server{}
{
}

void server::add_controller(const std::string& resource, controller* ctrl)
{
    const std::string collection{"^/" + resource + "$"};
    const std::string item{"^/" + resource + "/([0-9a-f]+)$"};

    on(web::http::methods::GET, collection, ctrl->bind_handler(&controller::get));
    on(web::http::methods::POST, collection, ctrl->bind_handler(&controller::post));

    on(web::http::methods::GET, item, ctrl->bind_handler(&controller::get_item));
    on(web::http::methods::PUT, item, ctrl->bind_handler(&controller::put));
    on(web::http::methods::PATCH, item, ctrl->bind_handler(&controller::patch));
    on(web::http::methods::DEL, item, ctrl->bind_handler(&controller::del));

    ctrl->install(this);
}

void server::register_adapter(adapter* adpt)
{
    assert(adpt);

    adpt->install(this);
}

} // namespace rest
} // namespace http
} // namespace ops
