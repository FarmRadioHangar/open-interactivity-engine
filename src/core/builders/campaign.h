///
/// \file campaign.h
///
#pragma once

#include <nlohmann/json.hpp>
#include "../../ops/mongodb/bson/builder.h"

namespace core
{
    struct campaigns
    {
        static auto constexpr name = "campaigns";
    };

    class campaign_builder : public ops::mongodb::bson::builder
    {
    public:
        explicit campaign_builder(const nlohmann::json& j);
    };
}
