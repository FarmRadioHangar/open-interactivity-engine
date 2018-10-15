///
/// \file server.h
///
#pragma once

#include "../server.h"
#include "controller.h"

namespace ops
{
    namespace http
    {
        namespace rest
        {
            class server : public http::server
            {
            public:
                server();

                void add_controller(const std::string& resource, 
                                    controller* ctrl);
            };
        }
    }
}
