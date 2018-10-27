///
/// \file session_builder.h
///
#pragma once

#include <nlohmann/json.hpp>
#include "../ops/mongodb/bson/builder.h"

namespace nexmo
{
    struct session
    {
        static auto constexpr name = "nexmoSession";
    };

    class session_builder : public ops::mongodb::bson::builder
    {
    public:
        explicit session_builder(const nlohmann::json& j);
    };
}
