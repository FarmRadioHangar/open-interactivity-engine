///
/// \file content.h
///
#pragma once

#include <nlohmann/json.hpp>
#include "../mongodb/bson/builder.h"

namespace ops
{
    struct content
    {
        static auto constexpr name = "content";
    };

    class content_builder : public mongodb::bson::builder
    {
    public:
        explicit content_builder(const nlohmann::json& j);
    };
}
