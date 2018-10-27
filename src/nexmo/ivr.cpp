#include "ivr.h"
#include "../core/builders/content.h"
#include "../core/builders/media.h"
#include "../ops/mongodb/document.h"
#include "../ops/util/json.h"

namespace nexmo
{

using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::basic::make_document;

script::script(const nlohmann::json& j)
{
    const auto& nodes = j.find("nodes");

    if (j.end() != nodes) {
        for (nlohmann::json::const_iterator i = nodes->begin(); i != nodes->end(); ++i) {
            const auto& key = i.key();
            const auto& j_node = i.value();
            const auto& type = j_node["type"];
            if ("transmit" == type) {
                node_transmit transmit;
                transmit.type = t_transmit;
                transmit.content = j_node["content"];
                this->nodes.insert({key, std::make_shared<node_transmit>(transmit)});
            } else if ("select" == type) {
                node_select select;
                select.type = t_select;
                for (const auto& key : j_node["keys"]) 
                    select.keys.push_back(key);
                this->nodes.insert({key, std::make_shared<node_select>(select)});
            } else if ("receive" == type) {
                node receive;
                receive.type = t_receive;
                this->nodes.insert({key, std::make_shared<node>(receive)});
            }
        }
    }

    const auto& edges = j.find("edges");

    if (j.end() != edges) {
        for (const auto& edge : *edges) {
            const std::string& src = edge.at("source");
            const std::string& dest = edge.at("dest");
            if (this->edges.count(src)) {
                this->edges.at(src).push_back(dest);
            } else {
                this->edges.insert({src, {dest}});
            }
        }
    }

    root = std::string{j.at("root")};
}

ivr::ivr(const script& script)
  : _script{script},
    _ncco{}
{
}

ivr::ivr(const nlohmann::json& j)
  : _script{script{j}},
    _ncco{}
{
}

void ivr::generate_ncco(std::string key)
{
    bool running = true;
    int i = 0;

    while (running && i++ < 256)
    {
        auto node = _script.nodes.at(key);

        //auto edges = _script.edges.count(key) 
        //    ? _script.edges.at(key) 
        //    : std::list<std::string>{};

        if (t_transmit == node->type)
        {
            node_transmit* transmit = static_cast<node_transmit*>(node.get());

            const std::string& content_id = transmit->content;

            auto doc = ops::mongodb::document<core::content>::find("id", content_id);
            auto j_content = ops::util::json::extract(doc);

            const auto& j_audio = j_content["reps"]["audio/mpeg"]["en"]; // todo
            const std::string& media_id = j_audio["media"];

            const std::string& url = "http://localhost:9080/media/" + media_id;

            _ncco.push_back({
                {"action", "stream"},
                {"streamUrl", { url }}
            });

            if (_script.edges.count(key) && 1 == _script.edges.at(key).size()) {
                key = _script.edges.at(key).front();
            } else {
                running = false;
            }
        }
        else if (t_select == node->type)
        {
//            node_select* select = static_cast<node_select*>(node.get());
//            const std::list<std::string>& keys = select->keys;

            const std::string& url = "http://localhost:9080/nexmo/response/n/" + key;

            _ncco.push_back({
                {"action", "input"},
                {"eventUrl", { url }}
            });

            running = false;
        }
        else // t_receive
        {
//            struct node* receive = node.get();

            const std::string& url = "http://localhost:9080/nexmo/record/n/" + key;

            _ncco.push_back({
                {"action", "record"},
                {"eventUrl", { url }}
            });

            running = false;
        }
    }
}

} // namespace nexmo
