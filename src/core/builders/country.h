///
/// \file country.h
///
#pragma once

#include <nlohmann/json.hpp>
#include "../../ops/mongodb/bson/builder.h"

namespace core
{
    class country : public ops::mongodb::bson::builder
    {
    public:
        static auto constexpr name = "countries";

        explicit country(const nlohmann::json& j);
    };
}
