///
/// \file adapter.h
///
#pragma once

#include <nlohmann/json.hpp>
#include "../../ops/mongodb/bson/builder.h"

namespace core
{
    class adapter : public ops::mongodb::bson::builder
    {
    public:
        static auto constexpr name = "adapters";

        explicit adapter(const nlohmann::json& j);

    private:
        std::string _module;
    };
}
