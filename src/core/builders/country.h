///
/// \file country.h
///
#pragma once

#include <nlohmann/json.hpp>
#include "../../ops/mongodb/bson/builder.h"

namespace ops
{
    class country_builder : public mongodb::bson::builder
    {
    public:
        explicit country_builder(const nlohmann::json& j);
    };
}
