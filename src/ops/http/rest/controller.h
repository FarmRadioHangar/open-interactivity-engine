///
/// \file controller.h
///
#pragma once

#include "server.h"

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

        virtual void get_item(http::request& request);
        virtual void get(http::request& request);
        virtual void post(http::request& request);
        virtual void put(http::request& request);
        virtual void patch(http::request& request);
        virtual void del(http::request& request);

        template <typename T = controller>
        request::handler bind_handler(void (T::* handler)(http::request&));

        void install(rest::server* server);

    private:
        virtual void do_install(rest::server* server);
    };

    template <typename T>
    inline request::handler controller::bind_handler(void (T::* handler)(http::request&))
    {
        return std::bind(handler, static_cast<T*>(this), std::placeholders::_1);
    }

    inline void controller::install(rest::server* server)
    {
        do_install(server);
    }

    inline void controller::do_install(rest::server* server)
    {
    }
}
}
}
