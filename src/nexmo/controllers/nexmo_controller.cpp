#include "nexmo_controller.h"
#include <bsoncxx/builder/basic/document.hpp>
#include "../../core/builders/campaign.h"
#include "../../ops/mongodb/counter.h"
#include "../../ops/mongodb/document.h"
#include "../../ops/util/json.h"
#include "../builders/session_builder.h"
#include "../ivr.h"

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

    request.with_body([&request](const std::string& body)
    {
        const auto campaign_id = request.get_uri_param(1);
        const auto feature_id = request.get_uri_param(2);

        nlohmann::json j_session{};

        // {
        //   "conversation_uuid": "CON-c387139a-060f-4875-8f6e-6f64da0971df",
        //   "from": "xxxxxxxxxxxx",
        //   "to": "44xxxxxxxxxx",
        //   "uuid": "daaa34f8cfca9e70e6f3ea4091b5831c"
        // }
        j_session["nexmo"] = nlohmann::json::parse(body);
        j_session["id"] = ops::mongodb::counter::generate_id();
        j_session["campaign"] = campaign_id;

        auto doc = ops::mongodb::document<core::campaigns>::find("id", campaign_id);
        auto j_campaign = ops::util::json::extract(doc);

        j_session["feature"] = j_campaign["features"][feature_id];

        session_builder builder(j_session);

        ops::mongodb::document<nexmo::session>::create(builder.extract());

        //

        request.send_response();

        //const auto campaign_id = request.get_uri_param(1);
        //const auto feature_id = request.get_uri_param(2);

        //auto doc = ops::mongodb::document<ops::campaigns>::find("id", campaign_id);

        //auto j_campaign = ops::util::json::extract(doc);
        //auto j_feature = j_campaign["features"][feature_id];
        //auto j_adapter = j_feature["adapters"]["nexmo_voice"];

        //std::cout << j_feature.dump() << std::endl;
        //std::cout << j_adapter.dump() << std::endl;

        //if ("script" == j_feature["type"]) {

        //    script_runner runner(j_feature["data"]["graph"]);

        //    runner.generate_ncco(runner.root());

        //    std::cout << runner.ncco().dump() << std::endl;

        //}

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

    });
}

void controller::post_response(ops::http::request& request)
{
    std::cout << "controller::post_response" << std::endl;
}

void controller::post_record(ops::http::request& request)
{
    std::cout << "controller::post_record" << std::endl;
}

void controller::do_install(ops::http::rest::server* server)
{
    server->on(methods::POST, "^/nexmo/event/c/([0-9a-f]+)/n/([0-9]+)$",
        bind_handler<controller>(&controller::post_event));

    server->on(methods::POST, "^/nexmo/answer/c/([0-9a-f]+)/f/([0-9a-f]+)$",
        bind_handler<controller>(&controller::post_answer));

    server->on(methods::POST, "^/nexmo/response/c/([0-9a-f]+)/n/(\\w)+$",
        bind_handler<controller>(&controller::post_response));

    server->on(methods::POST, "^/nexmo/record/c/([0-9a-f]+)/n/(\\w)+$",
        bind_handler<controller>(&controller::post_record));
}

} // namespace nexmo
