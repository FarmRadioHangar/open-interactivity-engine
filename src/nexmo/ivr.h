///
/// \file ivr.h
///
#pragma once

#include <cassert>
#include <map>
#include <nlohmann/json.hpp>
#include <utility>
#include <vector>

namespace nexmo
{
namespace ivr
{
    enum node_type
    {
        t_transmit,
        t_select,
        t_receive
    };

    struct node
    {
        node_type type;
    };

    struct node_transmit : public node
    {
        std::string content;
    };

    struct node_select : public node
    {
        std::vector<std::string> keys;
    };

    class script
    {
    public:
        using node_map = std::map<std::string, std::shared_ptr<node>>;
        using edge_map = std::map<std::string, std::vector<std::string>>;

        script(const nlohmann::json& j, const std::string& node_key);

        std::shared_ptr<node> current_node() const;
        bool traverse_edge(int n);

        nlohmann::json build_ncco(const std::string& session_id);

    private:
        std::string get_media_url(const std::string& host, 
                                  const std::string& content_id) const;

        std::string _node_key;
        node_map    _nodes;
        edge_map    _edges;
    };

    inline std::shared_ptr<node> script::current_node() const
    {
        assert(_nodes.count(_node_key));

        return _nodes.at(_node_key);
    }

    inline bool script::traverse_edge(int n)
    {
        if (!_edges.count(_node_key)) {
            return false;
        }

        _node_key = _edges.at(_node_key).at(n);
        return true;
    }
}
}
