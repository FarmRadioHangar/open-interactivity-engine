///
/// \file media.h
///
#pragma once

#include <nlohmann/json.hpp>
#include "../../ops/mongodb/bson/builder.h"

namespace core
{
    struct media
    {
        static auto constexpr name = "media";
    };

    class media_builder : public ops::mongodb::bson::builder
    {
    public:
        explicit media_builder(const nlohmann::json& j);
    };
}
