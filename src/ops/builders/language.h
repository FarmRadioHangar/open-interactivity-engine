///
/// \file language.h
///
#pragma once

#include <nlohmann/json.hpp>
#include "../mongodb/bson/builder.h"

namespace ops
{
    class language_builder : public mongodb::bson::builder
    {
    public:
        explicit language_builder(const nlohmann::json& j);
    };
}
