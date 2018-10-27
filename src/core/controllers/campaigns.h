///
/// \file campaigns.h
///
#pragma once

#include "../../ops/http/rest/controller.h"

namespace ops
{
    namespace http
    {
        namespace rest
        {
            class server;
        }
    }

    class campaigns_controller : public http::rest::controller
    {
    public:
        campaigns_controller();

        void get_item(http::request& request) override;
        void get(http::request& request) override;
        void post(http::request& request) override;

        void post_feature(http::request& request);
        void patch_feature(http::request& request);
        void post_language(http::request& request);
        void post_adapter(http::request& request);

    private:
        void do_install(http::rest::server* server) override;
    };
}
