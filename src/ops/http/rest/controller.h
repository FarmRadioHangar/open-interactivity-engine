///
/// \file controller.h
///
#pragma once

#include "../server.h"

namespace ops
{
    namespace http
    {
        namespace rest
        {
            class controller
            {
            public:
                controller();

                virtual ~controller() = default;

                controller(const controller&) = delete;
                controller& operator=(const controller&) = delete;

                virtual void get_item(http::request request);
                virtual void get(http::request request);
                virtual void post(http::request request);
                virtual void put(http::request request);
                virtual void patch(http::request request);
                virtual void del(http::request request);

                request::handler bind_handler(void (http::rest::controller::* handler)(http::request));
            };

            inline request::handler controller::bind_handler(void (http::rest::controller::* handler)(http::request))
            {
                return std::bind(handler, this, std::placeholders::_1);
            }
        }
    }
}
