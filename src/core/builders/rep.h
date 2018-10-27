///
/// \file rep.h
///
#pragma once

#include <nlohmann/json.hpp>
#include "../../ops/mongodb/bson/builder.h"

namespace core
{
    class rep_builder : public ops::mongodb::bson::builder
    {
    public:
        explicit rep_builder(const nlohmann::json& j);
    };
}
