///
/// \file countries.h
///
#pragma once

#include "../../ops/http/rest/controller.h"

namespace core
{
    class countries_controller : public ops::http::rest::controller
    {
    public:
        countries_controller();
    };
}
