///
/// \file script_runner.h
///
#pragma once

#include <nlohmann/json.hpp>
#include <list>
#include <map>
#include <utility>

namespace nexmo
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

    struct script
    {
        using node_map = std::map<std::string, std::shared_ptr<node>>;
        using edge_map = std::map<std::string, std::list<std::string>>;

        node_map    nodes;
        edge_map    edges;
        std::string root;

        explicit script(const nlohmann::json& j);
    };

    class script_runner
    {
    public:
        explicit script_runner(const script& script);
        explicit script_runner(const nlohmann::json& j);

        std::string root() const;

        void generate_ncco(std::string key);
        nlohmann::json ncco() const;

    private:
        const script   _script;
        nlohmann::json _ncco;
    };

    inline std::string script_runner::root() const
    {
        return _script.root;
    }

    inline nlohmann::json script_runner::ncco() const
    {
        return _ncco;
    }
}
