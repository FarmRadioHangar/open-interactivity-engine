///
/// \file countries.h
///
#pragma once

#include "../../ops/http/rest/controller.h"

namespace ops
{
    class countries_controller : public http::rest::controller
    {
    public:
        countries_controller();
    };
}
