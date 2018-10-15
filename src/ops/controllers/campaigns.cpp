#include "campaigns.h"

namespace ops
{

campaigns_controller::campaigns_controller()
{
}

void campaigns_controller::get_item(http::request request)
{
    std::cout << "campaigns_controller::get_item" << std::endl;
}

void campaigns_controller::get(http::request request)
{
    std::cout << "campaigns_controller::get" << std::endl;
}

void campaigns_controller::post(http::request request)
{
    std::cout << "campaigns_controller::post" << std::endl;
}

void campaigns_controller::post_feature(http::request request)
{
    std::cout << "campaigns_controller::post_feature" << std::endl;
}

void campaigns_controller::post_language(http::request request)
{
    std::cout << "campaigns_controller::post_language" << std::endl;
}

} // namespace ops
