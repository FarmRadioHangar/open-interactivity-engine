///
/// \file content.h
///
#pragma once

#include <nlohmann/json.hpp>
#include "../../ops/mongodb/bson/builder.h"

namespace core
{
    class content : public ops::mongodb::bson::builder
    {
    public:
        static auto constexpr name = "content";

        explicit content(const nlohmann::json& j);
    };
}
