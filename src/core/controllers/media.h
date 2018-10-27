///
/// \file media.h
///
#pragma once 

#include "../../ops/http/rest/controller.h"

namespace ops
{
    class media_controller : public http::rest::controller
    {
    public:
        media_controller();

        void get_item(http::request& request) override;
        void post(http::request& request) override;
    };
}
