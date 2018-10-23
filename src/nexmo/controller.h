///
/// \file controller.h
///
#pragma once

#include "../ops/http/rest/controller.h"

namespace nexmo
{
    class controller : public ops::http::rest::controller
    {
    public:
        controller();

        void test(ops::http::request request);

    private:
        void do_install(ops::http::rest::server* server) override;
    };
}
