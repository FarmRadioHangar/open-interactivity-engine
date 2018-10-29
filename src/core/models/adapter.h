///
/// \file adapter.h
///
#pragma once

#include <nlohmann/json.hpp>
#include <optional>
#include "../../ops/mongodb/model.h"

namespace core
{
    class adapter : public ops::mongodb::model<adapter>
    {
    public:
        static auto constexpr collection = "adapters";

        explicit adapter(const nlohmann::json& j);

        std::string module() const;

    private:
        bsoncxx::document::view get_bson() const;

        std::string                   _module;
        std::optional<nlohmann::json> _data;
    };

    inline std::string adapter::module() const
    {
        return _module;
    }
}
