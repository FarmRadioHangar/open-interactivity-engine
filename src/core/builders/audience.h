///
/// \file audience.h
///
#pragma once

#include <nlohmann/json.hpp>
#include "../../ops/mongodb/bson/builder.h"

namespace core
{
    class audience_builder : public ops::mongodb::bson::builder
    {
    public:
        explicit audience_builder(const nlohmann::json& j);
    };
}
