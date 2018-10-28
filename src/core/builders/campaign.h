///
/// \file campaign.h
///
#pragma once

#include <nlohmann/json.hpp>
#include "../../ops/mongodb/bson/builder.h"

namespace core
{
    class campaign : public ops::mongodb::bson::builder
    {
    public:
        static auto constexpr name = "campaigns";

        explicit campaign(const nlohmann::json& j);

    private:
        std::string _id;
        std::string _name;
        std::string _alias;
    };
}
