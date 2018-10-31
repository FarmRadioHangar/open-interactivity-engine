///
/// \file ivr.h
///
#pragma once

#include <nlohmann/json.hpp>
#include <list>
#include <map>
#include <utility>

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
        std::list<std::string> keys;
    };

    class script
    {
    public:
        using node_map = std::map<std::string, std::shared_ptr<node>>;
        using edge_map = std::map<std::string, std::list<std::string>>;

        explicit script(const nlohmann::json& j);

        std::shared_ptr<node> next_node();

        std::string node_key() const;
        bool has_next() const;

    private:
        std::string _node;
        node_map    _nodes;
        edge_map    _edges;
        bool        _has_next;
    };

    inline std::string script::node_key() const
    {
        return _node;
    }

    inline bool script::has_next() const
    {
        return _has_next;
    }
}
}
