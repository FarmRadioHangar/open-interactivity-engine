///
/// \file feature.h
///
#pragma once

#include <nlohmann/json.hpp>
#include "../../ops/mongodb/bson/builder.h"

namespace core
{
    class feature : public ops::mongodb::bson::builder
    {
    public:
        explicit feature(const nlohmann::json& j);
    };
}
