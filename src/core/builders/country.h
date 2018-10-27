///
/// \file country.h
///
#pragma once

#include <nlohmann/json.hpp>
#include "../../ops/mongodb/bson/builder.h"

namespace core
{
    class country_builder : public ops::mongodb::bson::builder
    {
    public:
        explicit country_builder(const nlohmann::json& j);
    };
}
