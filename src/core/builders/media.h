///
/// \file media.h
///
#pragma once

#include <nlohmann/json.hpp>
#include "../../ops/mongodb/bson/builder.h"

namespace core
{
    class media : public ops::mongodb::bson::builder
    {
    public:
        static auto constexpr name = "media";

        explicit media(const nlohmann::json& j);
    };
}
