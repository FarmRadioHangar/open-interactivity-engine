///
/// \file campaign.h
///
#pragma once

#include <nlohmann/json.hpp>
#include "../../ops/mongodb/bson/builder.h"

namespace ops
{
    struct campaigns
    {
        static auto constexpr name = "campaigns";
    };

    class campaign_builder : public mongodb::bson::builder
    {
    public:
        explicit campaign_builder(const nlohmann::json& j);
    };
}
