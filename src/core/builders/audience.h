///
/// \file audience.h
///
#pragma once

#include <nlohmann/json.hpp>
#include "../../ops/mongodb/bson/builder.h"

namespace core
{
    class audience : public ops::mongodb::bson::builder
    {
    public:
        static auto constexpr name = "audience";

        explicit audience(const nlohmann::json& j);
    };
}
