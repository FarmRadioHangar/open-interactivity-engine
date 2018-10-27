///
/// \file feature.h
///
#pragma once

#include <nlohmann/json.hpp>
#include "../../ops/mongodb/bson/builder.h"

namespace core
{
    class feature_builder : public ops::mongodb::bson::builder
    {
    public:
        explicit feature_builder(const nlohmann::json& j);
    };
}
