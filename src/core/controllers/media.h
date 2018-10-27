///
/// \file media.h
///
#pragma once 

#include "../../ops/http/rest/controller.h"

namespace core
{
    class media_controller : public ops::http::rest::controller
    {
    public:
        media_controller();

        void get_item(ops::http::request& request) override;
        void post(ops::http::request& request) override;
    };
}
