///
/// \file campaigns.h
///
#pragma once

#include "../../ops/http/rest/controller.h"

namespace core
{
    namespace http
    {
        namespace rest
        {
            class server;
        }
    }

    class campaigns_controller : public ops::http::rest::controller
    {
    public:
        campaigns_controller();

        void get_item(ops::http::request& request) override;
        void get(ops::http::request& request) override;
        void post(ops::http::request& request) override;

        void post_feature(ops::http::request& request);
        void patch_feature(ops::http::request& request);
        void post_language(ops::http::request& request);
        void post_adapter(ops::http::request& request);

    private:
        void do_install(ops::http::rest::server* server) override;
    };
}
