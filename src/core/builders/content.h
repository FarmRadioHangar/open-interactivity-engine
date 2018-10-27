///
/// \file content.h
///
#pragma once

#include <nlohmann/json.hpp>
#include "../../ops/mongodb/bson/builder.h"

namespace core
{
    struct content
    {
        static auto constexpr name = "content";
    };

    class content_builder : public ops::mongodb::bson::builder
    {
    public:
        explicit content_builder(const nlohmann::json& j);
    };
}
