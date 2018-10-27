///
/// \file language.h
///
#pragma once

#include <nlohmann/json.hpp>
#include "../../ops/mongodb/bson/builder.h"

namespace ops
{
    struct languages
    {
        static auto constexpr name = "languages";
    };

    class language_builder : public mongodb::bson::builder
    {
    public:
        explicit language_builder(const nlohmann::json& j);
    };
}
