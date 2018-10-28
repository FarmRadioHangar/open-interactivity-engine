///
/// \file language.h
///
#pragma once

#include <nlohmann/json.hpp>
#include "../../ops/mongodb/bson/builder.h"

namespace core
{
    class language : public ops::mongodb::bson::builder
    {
    public:
        static auto constexpr name = "languages";

        explicit language(const nlohmann::json& j);
    };
}
