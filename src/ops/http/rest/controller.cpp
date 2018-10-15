#include "controller.h"

namespace ops
{
namespace http
{
namespace rest
{

controller::controller()
{
}

void controller::get_item(http::request request)
{
    std::cout << "get_item" << std::endl;
}

void controller::get(http::request request)
{
    std::cout << "get" << std::endl;
}

void controller::post(http::request request)
{
    std::cout << "post" << std::endl;
}

void controller::put(http::request request)
{
    std::cout << "put" << std::endl;
}

void controller::patch(http::request request)
{
    std::cout << "patch" << std::endl;
}

void controller::del(http::request request)
{
    std::cout << "del" << std::endl;
}

} // namespace rest
} // namespace http
} // namespace ops
