#include "campaigns.h"
#include <nlohmann/json.hpp>
#include "../mongodb/document.h"
#include "../mongodb/page.h"

struct campaigns
{
    static auto constexpr name = "campaigns";
};

namespace ops
{
    namespace util
    {
        namespace json
        {
            template <typename T>
            nlohmann::json builder(const mongodb::document<T>& doc)
            {
                nlohmann::json j;
                doc.stream() >> j;

                j.erase("_id");

                return j;
            }
        }
    }
}

namespace ops
{

using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::basic::make_document;

campaigns_controller::campaigns_controller()
  : http::rest::controller{}
{
}

void campaigns_controller::get_item(http::request request)
{
    const auto id = request.get_uri_param(1);
    const auto doc = mongodb::document<campaigns>::find(make_document(kvp("id", id)));

    nlohmann::json res;
    res["campaign"] = util::json::builder(doc);

    request.send_response(res.dump());
}

void campaigns_controller::get(http::request request)
{
    std::cout << "campaigns_controller::get" << std::endl;

    const auto skip = request.get_query_param<int64_t>("skip", 0);
    const auto limit = request.get_query_param<int64_t>("limit", 10);

    //mongodb::page<campaigns> page{};

    nlohmann::json res;

    request.send_response(res.dump());
}

void campaigns_controller::post(http::request request)
{
    std::cout << "campaigns_controller::post" << std::endl;
    
    nlohmann::json res;

    request.send_response(res.dump());
}

void campaigns_controller::post_feature(http::request request)
{
    std::cout << "campaigns_controller::post_feature" << std::endl;

    nlohmann::json res;

    request.send_response(res.dump());
}

void campaigns_controller::post_language(http::request request)
{
    std::cout << "campaigns_controller::post_language" << std::endl;

    nlohmann::json res;

    request.send_response(res.dump());
}

} // namespace ops
