#include "nexmo_controller.h"
#include <bsoncxx/builder/basic/document.hpp>
#include "../../core/models/campaign.h"
#include "../../core/models/content.h"
#include "../../dotenv/dotenv.h"
#include "../../ops/mongodb/counter.h"
#include "../../ops/mongodb/document.h"
#include "../../ops/util/json.h"
#include "../ivr.h"
#include "../models/session.h"

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

        nexmo::ivr::script graph(j_session["feature"]["data"]["graph"], node_key);

        auto j = nlohmann::json::parse(body);

        // {
        //   "dtmf":"2",
        //   "timed_out": false,
        //   "uuid": null,
        //   "conversation_uuid": "CON-5584355f-9e68-4bc7-b272-92e5842c4711",
        //   "timestamp": "2018-10-31T08:11:33.044Z"
        // }
        //

        std::shared_ptr<ivr::node> n = graph.current_node();

        if (ivr::t_select == n->type) {
            const ivr::node_select* node = static_cast<ivr::node_select*>(n.get());
            size_t p = 0;
            for (auto i = node->keys.begin(); i != node->keys.end(); ++i) {
                if (j["dtmf"] == *i)
                    break;
                ++p;
            }
            if (p < node->keys.size()) {
                graph.traverse_edge(p);
            } 
        } else if (ivr::t_receive == n->type) {
            std::cout << "///////////////////////////////////////////////////////////////////" << std::endl;
            std::cout << j.dump() << std::endl;
            // todo
        }

        request.send_response(graph.build_ncco(session_id).dump());
    });
}

void controller::post_event_(ops::http::request& request)
{
    request.with_body([this, &request](const std::string& body)
    {
        std::cout << "-------------------------------------------------------------------" << std::endl;
        std::cout << body << std::endl;
        std::cout << "-------------------------------------------------------------------" << std::endl;
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

        // Register a new session
        ops::mongodb::document<nexmo::session>::create(model.builder().extract());

        const auto& j_graph = j_session["feature"]["data"]["graph"];
        nexmo::ivr::script graph(j_graph, j_graph["root"]);

        request.send_response(graph.build_ncco(session_id).dump());
    });
}

void controller::do_install(ops::http::rest::server* server)
{
    server->on(methods::POST, "^/nexmo/event/s/([0-9a-f]+)/n/([0-9]+)$",
        bind_handler<controller>(&controller::post_event));

    server->on(methods::POST, "^/nexmo/event$",
        bind_handler<controller>(&controller::post_event_));

    server->on(methods::POST, "^/nexmo/answer/c/([0-9a-f]+)/f/([0-9a-f]+)$",
        bind_handler<controller>(&controller::post_answer));
}

} // namespace nexmo
