///
/// \file rep.h
///
#pragma once

#include <nlohmann/json.hpp>
#include "../../ops/mongodb/bson/builder.h"

namespace core
{
    class rep : public ops::mongodb::bson::builder
    {
    public:
        explicit rep(const nlohmann::json& j);
    };
}
