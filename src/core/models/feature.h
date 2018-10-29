///
/// \file feature.h
///
#pragma once

#include <list>
#include <nlohmann/json.hpp>
#include <optional>
#include "../../ops/mongodb/model.h"
#include "adapter.h"

namespace core
{
    class feature : public ops::mongodb::model<feature>
    {
    public:
        explicit feature(const nlohmann::json& j);

        std::optional<std::string> id() const;

    private:
        bsoncxx::document::view get_bson() const;

        std::list<adapter>            _adapters;
        std::optional<nlohmann::json> _data;
        std::optional<std::string>    _id;
        std::string                   _type;
    };

    inline std::optional<std::string> feature::id() const
    {
        return _id;
    }
}
