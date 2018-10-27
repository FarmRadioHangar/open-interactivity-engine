///
/// \file languages.h
///
#pragma once

#include "../../ops/http/rest/controller.h"

namespace ops
{
    class languages_controller : public http::rest::controller
    {
    public:
        languages_controller();

        void get_item(http::request& request) override;
        void get(http::request& request) override;
        void post(http::request& request) override;
    };
}
