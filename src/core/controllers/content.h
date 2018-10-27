///
/// \file content.h
///
#pragma once

#include "../../ops/http/rest/controller.h"

namespace core
{
    class content_controller : public ops::http::rest::controller
    {
    public:
        content_controller();

        void get_item(ops::http::request& request) override;
        void get(ops::http::request& request) override;
        void post(ops::http::request& request) override;

        void post_rep(ops::http::request& request);

    private:
        void do_install(ops::http::rest::server* server) override;
    };
}
