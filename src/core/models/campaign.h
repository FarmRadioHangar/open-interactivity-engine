///
/// \file campaign.h
///
#pragma once

#include <list>
#include <nlohmann/json.hpp>
#include <optional>
#include "../../ops/mongodb/model.h"
#include "feature.h"
#include "language.h"

namespace core
{
    class campaign : public ops::mongodb::model<campaign>
    {
    public:
        static auto constexpr collection = "campaigns";

        explicit campaign(const nlohmann::json& j);

    private:
        bsoncxx::document::view get_bson() const;

        std::string                _name;
        std::optional<std::string> _id;
        std::optional<std::string> _alias;
        std::list<feature>         _features;
        std::list<language>        _languages;
    };
}
