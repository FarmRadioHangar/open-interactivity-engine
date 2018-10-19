///
/// \file adapter.h
///
#pragma once

#include <nlohmann/json.hpp>
#include "../mongodb/bson/builder.h"

namespace ops
{
    class adapter_builder : public mongodb::bson::builder
    {
    public:
        explicit adapter_builder(const nlohmann::json& j);
    };
}
