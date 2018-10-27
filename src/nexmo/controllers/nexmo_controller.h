///
/// \file nexmo_controller.h
///
#pragma once

#include "../../ops/http/rest/controller.h"

namespace nexmo
{
    class controller : public ops::http::rest::controller
    {
    public:
        controller();

        void post_event(ops::http::request& request);
        void post_answer(ops::http::request& request);
        void post_response(ops::http::request& request);
        void post_record(ops::http::request& request);

    private:
        void do_install(ops::http::rest::server* server) override;
    };
}
