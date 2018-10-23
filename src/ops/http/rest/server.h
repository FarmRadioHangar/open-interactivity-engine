///
/// \file server.h
///
#pragma once

#include "../server.h"
#include <cpprest/http_listener.h>
#include "../../adapter.h"

namespace ops
{
    namespace http
    {
        namespace rest
        {
            class controller;

            class server : public http::server
            {
            public:
                server();

                void add_controller(
                    const std::string& resource,
                    controller* ctrl);

                void add_route(
                    const web::http::method method,
                    const std::string& pattern,
                    request::handler handler);

                void register_adapter(adapter* adpt);
            };
        }
    }
}
