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

void controller::post_ivr(ops::http::request& request)
{
    request.with_body([this, &request](const std::string& body)
    {
        std::cout << "controller::post_ivr" << std::endl;

        const auto session_id = request.get_uri_param(1);
        const auto node_key   = request.get_uri_param(2);

        auto j_body = nlohmann::json::parse(body);

        auto session_doc = ops::mongodb::document<nexmo::session>::find("id", session_id);
        auto j_session = ops::util::json::extract(session_doc);

        nexmo::ivr::script graph(j_session["feature"]["data"]["graph"], node_key);

        std::shared_ptr<ivr::node> n = graph.current_node();

        if (ivr::t_select == n->type) {

            const ivr::node_select* node = static_cast<ivr::node_select*>(n.get());
            size_t p = 0;
            for (auto i = node->keys.begin(); i != node->keys.end(); ++i) {
                if (j_body["dtmf"] == *i)
                    break;
                ++p;
            }
            if (p < node->keys.size()) {
                graph.traverse_edge(p);
            } 
        } else if (ivr::t_receive == n->type) {

            // todo: process audio and create media

            request.send_response();
            return;
        }

        request.send_response(graph.build_ncco(session_id).dump());
    });
}

void controller::post_event(ops::http::request& request)
{
    request.with_body([this, &request](const std::string& body)
    {
        std::cout << "controller::post_event" << std::endl;

        auto j_body = nlohmann::json::parse(body);

        const std::string uuid = j_body["conversation_uuid"];

        auto collection = ops::mongodb::pool::instance().database().collection(nexmo::session::collection);

        const auto filter = make_document(kvp("conversation.conversation_uuid", uuid));

        mongocxx::options::find_one_and_update options{};
        options.upsert(true);

        bsoncxx::builder::basic::document builder{};
        builder.append(kvp("$push", [this, body](bsoncxx::builder::basic::sub_document sub_builder) {
            sub_builder.append(kvp("events", bsoncxx::from_json(body)));
        }));

        collection.find_one_and_update(filter.view(), builder.extract(), options);

        request.send_response();
    });
}

void controller::post_answer(ops::http::request& request)
{
    request.with_body([this, &request](const std::string& body)
    {
        std::cout << "controller::post_answer" << std::endl;

        auto j_body = nlohmann::json::parse(body);

        const std::string campaign_id = request.get_uri_param(1);
        const std::string feature_id  = request.get_uri_param(2);

        auto campaign_doc = ops::mongodb::document<core::campaign>::find("id", campaign_id);
        auto j_campaign = ops::util::json::extract(campaign_doc);

        const std::string session_id = ops::mongodb::counter::generate_id();

        const std::string uuid = j_body["conversation_uuid"];

        auto collection = ops::mongodb::pool::instance().database().collection(nexmo::session::collection);

        std::cout << "uuid: " << uuid << std::endl;

        const auto filter = make_document(kvp("conversation.conversation_uuid", uuid));

        mongocxx::options::find_one_and_update options{};
        options.upsert(true);

        bsoncxx::builder::basic::document builder{};

        builder.append(kvp("$set", [this, campaign_id, session_id, feature_id, body, 
              j_campaign](bsoncxx::builder::basic::sub_document update_builder) 
        {
            update_builder.append(kvp("id", session_id));

            update_builder.append(kvp("campaign", [this, campaign_id](bsoncxx::builder::basic::sub_document sub_builder) {
                sub_builder.append(kvp("id", campaign_id));
            }));

            update_builder.append(kvp("conversation", bsoncxx::from_json(body)));

            core::feature feature(j_campaign["features"][feature_id]);

            update_builder.append(kvp("feature", feature.builder().extract()));
        }));

        collection.find_one_and_update(filter.view(), builder.extract(), options);

        //session model(j_session);

        const auto j_graph = j_campaign["features"][feature_id]["data"]["graph"];
        nexmo::ivr::script graph(j_graph, j_graph["root"]);

        const auto j_resp = graph.build_ncco(session_id);

        request.send_response(j_resp.dump());
    });
}

void controller::do_install(ops::http::rest::server* server)
{
    server->on(methods::POST, "^/nexmo/ivr/s/([0-9a-f]+)/n/([0-9]+)$",
        bind_handler<controller>(&controller::post_ivr));

    server->on(methods::POST, "^/nexmo/event$",
        bind_handler<controller>(&controller::post_event));

    server->on(methods::POST, "^/nexmo/answer/c/([0-9a-f]+)/f/([0-9a-f]+)$",
        bind_handler<controller>(&controller::post_answer));
}

} // namespace nexmo
