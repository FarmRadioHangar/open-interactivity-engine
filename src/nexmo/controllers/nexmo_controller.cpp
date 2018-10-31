#include "nexmo_controller.h"
#include <bsoncxx/builder/basic/document.hpp>
#include "../../core/models/campaign.h"
#include "../../core/models/content.h"
#include "../../ops/mongodb/counter.h"
#include "../../ops/mongodb/document.h"
#include "../../ops/util/json.h"
#include "../ivr.h"
#include "../models/session.h"
#include "../../dotenv/dotenv.h"

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
    request.with_body([this, &request](const std::string& body)
    {
        const auto session_id = request.get_uri_param(1);
        const auto node_key = request.get_uri_param(2);

        auto session_doc = ops::mongodb::document<nexmo::session>::find("id", session_id);
        auto j_session = ops::util::json::extract(session_doc);

        nexmo::ivr::script graph(j_session["feature"]["data"]["graph"]);

        // {
        //   "dtmf":"",
        //   "timed_out": true,
        //   "uuid": null,
        //   "conversation_uuid": "CON-5584355f-9e68-4bc7-b272-92e5842c4711",
        //   "timestamp": "2018-10-31T08:11:33.044Z"
        // }

        std::cout << body << std::endl;

        //

        request.send_response();

    });
}

void controller::post_answer(ops::http::request& request)
{
    request.with_body([this, &request](const std::string& body)
    {
        const auto campaign_id = request.get_uri_param(1);
        const auto feature_id  = request.get_uri_param(2);

        const std::string session_id = ops::mongodb::counter::generate_id();

        nlohmann::json j_session{
            {"id", session_id},
            {"campaign", { {"id", campaign_id}} },
            {"conversation", nlohmann::json::parse(body)}
        };

        // {
        //   "conversation_uuid": "CON-c387139a-060f-4875-8f6e-6f64da0971df",
        //   "from": "xxxxxxxxxxxx",
        //   "to": "44xxxxxxxxxx",
        //   "uuid": "daaa34f8cfca9e70e6f3ea4091b5831c"
        // }

        auto campaign_doc = ops::mongodb::document<core::campaign>::find("id", campaign_id);
        auto j_campaign = ops::util::json::extract(campaign_doc);

        j_session["feature"] = j_campaign["features"][feature_id];

        session model(j_session);

        ops::mongodb::document<nexmo::session>::create(model.builder().extract());

        nexmo::ivr::script graph(j_session["feature"]["data"]["graph"]);

        int i = 0;
        auto ncco = nlohmann::json::array();

        while (graph.has_next() && i++ < 100)
        {
            std::shared_ptr<ivr::node> next = graph.next_node();
            const std::string host = dotenv::getenv("HOST", "http://localhost:9080");
            if (ivr::t_transmit == next->type) {
                const ivr::node_transmit* node = static_cast<ivr::node_transmit*>(next.get());
                ncco.push_back({
                    {"action", "stream"},
                    {"streamUrl", { get_media_url(host, node->content) }}
                });
            } else if (ivr::t_select == next->type) {
                std::string url = host + "/nexmo/event/s/" + session_id + "/n/" + graph.node_key();
                ncco.push_back({
                    {"action", "input"},
                    {"eventUrl", { url }}
                });
            } else {
                std::string url = host + "/nexmo/event/s/" + session_id + "/n/" + graph.node_key();
                ncco.push_back({
                    {"action", "record"},
                    {"eventUrl", { url }}
                });
            }
        }

        request.send_response(ncco.dump());
    });
}

void controller::do_install(ops::http::rest::server* server)
{
    server->on(methods::POST, "^/nexmo/event/s/([0-9a-f]+)/n/([0-9]+)$",
        bind_handler<controller>(&controller::post_event));

    server->on(methods::POST, "^/nexmo/answer/c/([0-9a-f]+)/f/([0-9a-f]+)$",
        bind_handler<controller>(&controller::post_answer));
}

std::string controller::get_media_url(const std::string& host, const std::string& content_id)
{
    auto content_doc = ops::mongodb::document<core::content>::find("id", content_id);
    auto j_content = ops::util::json::extract(content_doc);

    const auto& j_audio = j_content["reps"]["audio/mpeg"]["en"]; // todo

    return host + "/media/" + std::string{j_audio["media"]["id"]};
}

} // namespace nexmo
