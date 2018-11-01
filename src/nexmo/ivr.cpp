#include "ivr.h"
#include "../core/models/content.h"
#include "../core/models/media.h"
#include "../ops/mongodb/document.h"
#include "../ops/util/json.h"
#include "../dotenv/dotenv.h"

namespace nexmo
{

using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::basic::make_document;

ivr::script::script(const nlohmann::json& j, const std::string& node_key)
  : _node_key{node_key}
{
    const auto& nodes = j.find("nodes");
    const auto& edges = j.find("edges");

    if (j.end() != nodes) {
        for (nlohmann::json::const_iterator i = nodes->begin(); i != nodes->end(); ++i) {
            const auto& key = i.key();
            const auto& j_node = i.value();
            const auto& type = j_node["type"];
            if ("transmit" == type) {
                node_transmit node;
                node.type = t_transmit;
                node.content = j_node["content"];
                _nodes.insert({key, std::make_shared<node_transmit>(node)});
            } else if ("select" == type) {
                node_select node;
                node.type = t_select;
                for (const auto& key : j_node["keys"]) 
                    node.keys.push_back(key);
                _nodes.insert({key, std::make_shared<node_select>(node)});
            } else if ("receive" == type) {
                ivr::node node;
                node.type = t_receive;
                _nodes.insert({key, std::make_shared<ivr::node>(node)});
            }
        }
    }

    if (j.end() != edges) {
        for (const auto& edge : *edges) {
            const std::string& src = edge.at("source");
            const std::string& dest = edge.at("dest");
            if (_edges.count(src)) {
                _edges.at(src).push_back(dest);
            } else {
                _edges.insert({src, {dest}});
            }
        }
    }
}

nlohmann::json ivr::script::build_ncco(const std::string& session_id)
{
    auto ncco = nlohmann::json::array();

    const std::string host = dotenv::getenv("HOST", "http://localhost:9080");

    bool has_content = true;
    int i = 0;

    while (has_content && i++ < 300)
    {
        std::shared_ptr<ivr::node> n = current_node();

        if (ivr::t_transmit == n->type) {
            const ivr::node_transmit* node = static_cast<ivr::node_transmit*>(n.get());
            ncco.push_back({
                {"action", "stream"},
                {"streamUrl", { get_media_url(host, node->content) }}
            });
            traverse_edge(0);
        } else if (ivr::t_select == n->type) {
            ncco.push_back({
                {"action", "input"},
                {"maxDigits", 2},
                {"timeOut", 10},
                {"eventUrl", { host + "/nexmo/ivr/s/" + session_id + "/n/" + _node_key }}
            });
            has_content = false;
        } else {
            ncco.push_back({
                {"action", "record"},
                {"endOnKey", "#"},
                {"beepStart", true},
                {"endOnSilence", 10},
                {"eventUrl", { host + "/nexmo/ivr/s/" + session_id + "/n/" + _node_key }}
            });
            has_content = false;
        }
    }

    return ncco;
}

std::string ivr::script::get_media_url(const std::string& host, 
                                       const std::string& content_id) const
{
    auto content_doc = ops::mongodb::document<core::content>::find("id", content_id);
    auto j_content = ops::util::json::extract(content_doc);

    const auto& j_audio = j_content["reps"]["audio/mpeg"]["en"]; // todo

    return host + "/media/" + std::string{j_audio["media"]["id"]};
}

} // namespace nexmo
