///
/// \file session.h
///
#pragma once

#include <nlohmann/json.hpp>
#include <optional>
#include "../../ops/mongodb/model.h"
#include "../../core/models/campaign.h"
#include "../../core/models/feature.h"

namespace nexmo
{
    class session : public ops::mongodb::model<session>
    {
    public:
        static auto constexpr collection = "nexmoSession";

        explicit session(const nlohmann::json& j);

    private:
        bsoncxx::builder::basic::document get_builder() const;

        std::optional<std::string>   _id;
        std::optional<core::feature> _feature;
        std::string                  _campaign_id;
        nlohmann::json               _nexmo;
    };
}
