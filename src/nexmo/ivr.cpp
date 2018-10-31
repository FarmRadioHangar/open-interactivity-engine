#include "ivr.h"
#include "../core/models/content.h"
#include "../core/models/media.h"
#include "../ops/mongodb/document.h"
#include "../ops/util/json.h"

namespace nexmo
{

using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::basic::make_document;

ivr::script::script(const nlohmann::json& j)
  : has_next{true}
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

    node_key = std::string{j.at("root")};
}

std::shared_ptr<ivr::node> ivr::script::next_node()
{
    std::shared_ptr<ivr::node> current = _nodes.at(node_key);

    if (t_transmit == current->type) {
        node_key = _edges.at(node_key).front();
    } else {
        has_next = false;
    }

    return current;
}

} // namespace nexmo
