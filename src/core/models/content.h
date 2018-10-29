///
/// \file content.h
///
#pragma once

#include <nlohmann/json.hpp>
#include <list>
#include "../../ops/mongodb/model.h"
#include "rep.h"

namespace core
{
    class content : public ops::mongodb::model<content>
    {
    public:
        static auto constexpr collection = "content";

        explicit content(const nlohmann::json& j);

    private:
        bsoncxx::document::view get_bson() const;

        std::string                _title;
        std::optional<std::string> _id;
        std::list<rep>             _reps;
    };
}
