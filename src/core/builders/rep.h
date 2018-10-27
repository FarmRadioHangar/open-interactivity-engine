///
/// \file rep.h
///
#pragma once

#include <nlohmann/json.hpp>
#include "../../ops/mongodb/bson/builder.h"

namespace ops
{
    class rep_builder : public mongodb::bson::builder
    {
    public:
        explicit rep_builder(const nlohmann::json& j);
    };
}
