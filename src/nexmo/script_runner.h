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
        using edge = std::pair<std::string, std::string>;
        using node_map = std::map<std::string, std::shared_ptr<node>>;

        node_map        nodes;
        std::list<edge> edges;
        std::string     root;

        explicit script(const nlohmann::json& j);
    };

    class script_runner
    {
    public:
        explicit script_runner(const script& script);
        explicit script_runner(const nlohmann::json& j);

        std::string root() const;

        nlohmann::json generate_ncco(const std::string& key);

    private:
        const script _script;
    };

    inline std::string script_runner::root() const
    {
        return _script.root;
    }
}