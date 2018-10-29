///
/// \file audience.h
///
#pragma once

#include <nlohmann/json.hpp>
#include "../../ops/mongodb/model.h"

namespace core
{
    class audience : public ops::mongodb::model<audience>
    {
    public:
        static auto constexpr collection = "audience";

        explicit audience(const nlohmann::json& j);

    private:
        bsoncxx::document::view get_bson() const;
    };
}
