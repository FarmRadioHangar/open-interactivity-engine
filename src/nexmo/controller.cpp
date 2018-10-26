#include "controller.h"
#include <bsoncxx/builder/basic/document.hpp>
#include "../ops/builders/campaign.h"
#include "../ops/mongodb/document.h"
#include "../ops/util/json.h"
#include "script_runner.h"

namespace nexmo
{

using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::basic::make_document;
using web::http::methods;

controller::controller()
  : ops::http::rest::controller{}
{
}

void controller::post_event(ops::http::request& request)
{
    std::cout << "controller::post_event" << std::endl;

   //

    request.send_response();
}

void controller::post_answer(ops::http::request& request)
{
    std::cout << "controller::post_answer" << std::endl;

    const auto campaign_id = request.get_uri_param(1);
    const auto feature_id = request.get_uri_param(2);

    auto doc = ops::mongodb::document<ops::campaigns>::find("id", campaign_id);

    auto j_campaign = ops::util::json::extract(doc);
    auto j_feature = j_campaign["features"][feature_id];
    auto j_adapter = j_feature["adapters"]["nexmo_voice"];

    std::cout << j_feature.dump() << std::endl;
    std::cout << j_adapter.dump() << std::endl;

    if ("script" == j_feature["type"]) {

        script_runner runner(j_feature["data"]["graph"]);

    }

    //

    //auto ncco = nlohmann::json::array();
    //
    //{
    //    nlohmann::json item;

    //    item["action"] = "talk";
    //    item["text"] = "this is a test";

    //    ncco.push_back(item);
    //}

    //request.send_response(ncco.dump());
}

void controller::post_response(ops::http::request& request)
{
    std::cout << "controller::post_response" << std::endl;
}

void controller::do_install(ops::http::rest::server* server)
{
    server->on(methods::POST, "^/nexmo/event/c/([0-9a-f]+)/n/([0-9]+)$",
        bind_handler<controller>(&controller::post_event));

    server->on(methods::POST, "^/nexmo/answer/c/([0-9a-f]+)/f/([0-9a-f]+)$",
        bind_handler<controller>(&controller::post_answer));
}

} // namespace nexmo
