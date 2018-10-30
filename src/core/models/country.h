///
/// \file country.h
///
#pragma once

#include <nlohmann/json.hpp>
#include "../../ops/mongodb/model.h"

namespace core
{
    class country : public ops::mongodb::model<country>
    {
    public:
        static auto constexpr collection = "countries";

        explicit country(const nlohmann::json& j);

    private:
        bsoncxx::builder::basic::document get_builder() const;
    };
}
