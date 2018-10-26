///
/// \file server.h
///
#pragma once

#include "../server.h"

namespace ops
{
    class adapter;

namespace http
{
namespace rest
{
    class controller;

    class server : public http::server
    {
    public:
        server();

        void add_controller(const std::string& resource, controller* ctrl);

        void register_adapter(adapter* adpt);
    };
}
}
}
