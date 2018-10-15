#include "server.h"

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

    const auto bind_handler = [ctrl](auto handler) {
        return std::bind(handler, ctrl, std::placeholders::_1);
    };

    on(web::http::methods::GET, collection, bind_handler(&controller::get));
    on(web::http::methods::POST, collection, bind_handler(&controller::post));
    on(web::http::methods::GET, item, bind_handler(&controller::get_item));
    on(web::http::methods::PUT, item, bind_handler(&controller::put));
    on(web::http::methods::PATCH, item, bind_handler(&controller::patch));
    on(web::http::methods::DEL, item, bind_handler(&controller::del));
}

} // namespace rest
} // namespace http
} // namespace ops
