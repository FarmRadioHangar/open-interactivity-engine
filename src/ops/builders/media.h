///
/// \file media.h
///
#pragma once

#include <nlohmann/json.hpp>
#include "../mongodb/bson/builder.h"

namespace ops
{
    struct media
    {
        static auto constexpr name = "media";
    };

    class media_builder : public mongodb::bson::builder
    {
    public:
        explicit media_builder(const nlohmann::json& j);
    };
}
