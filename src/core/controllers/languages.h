///
/// \file languages.h
///
#pragma once

#include "../../ops/http/rest/controller.h"

namespace core
{
    class languages_controller : public ops::http::rest::controller
    {
    public:
        languages_controller();

        void get_item(ops::http::request& request) override;
        void get(ops::http::request& request) override;
        void post(ops::http::request& request) override;
    };
}
