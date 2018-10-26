#include "script_runner.h"
#include "../ops/mongodb/document.h"
#include "../ops/builders/content.h"
#include "../ops/builders/media.h"
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
                for (const auto& key : j_node["keys"]) {
                    select.keys.push_back(key);
                }
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
            std::pair<std::string, std::string> pair;
            pair.first = edge.at("source");
            pair.second = edge.at("dest");
            this->edges.emplace_back(pair);
        }
    }

    root = std::string{j.at("root")};
}

script_runner::script_runner(const script& script)
  : _script{script},
    _ncco{nlohmann::json::array()}
{
}

script_runner::script_runner(const nlohmann::json& j)
  : _script{script{j}},
    _ncco{nlohmann::json::array()}
{
}

void script_runner::generate_ncco(const std::string& key)
{
    auto node = _script.nodes.at("_root" == key ? root() : key);

    if (t_transmit == node->type)
    {
        node_transmit* transmit = static_cast<node_transmit*>(node.get());

        const std::string& content_id = transmit->content;

        std::cout << content_id << std::endl;

        auto doc = ops::mongodb::document<ops::content>::find("id", content_id);
        auto j_content = ops::util::json::extract(doc);

        const auto& j_audio = j_content["reps"]["audio/mpeg"]["en"]; // todo
        const std::string& media_id = j_audio["media"];

        //auto media = ops::mongodb::document<ops::media>::find("id", media_id);

        //auto j_media = ops::util::json::extract(media);
        //const std::string& filename = j_media["file"];

        const std::string& url = "http://localhost:9080/media/" + media_id;

        _ncco.push_back({
            {"action", "stream"},
            {"streamUrl", { url }}
        });
    }
    else if (t_select == node->type)
    {
        node_select* select = static_cast<node_select*>(node.get());

        const std::list<std::string>& keys = select->keys;
    }
    else // t_receive
    {
        struct node* receive = node.get();
    }
}

} // namespace nexmo
