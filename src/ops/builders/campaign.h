///
/// \file campaign.h
///
#pragma once

#include <nlohmann/json.hpp>
#include "../mongodb/bson/builder.h"

namespace ops
{
    class campaign_builder : public mongodb::bson::builder
    {
    public:
        explicit campaign_builder(const nlohmann::json& j);
    };
}
