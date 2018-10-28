///
/// \file language.h
///
#pragma once

#include <nlohmann/json.hpp>
#include "../../ops/mongodb/bson/builder.h"
#include "../../ops/mongodb/document.h"

namespace core
{
    class language
    {
    public:
        static auto constexpr name = "languages";

        explicit language(const nlohmann::json& j);

        ops::mongodb::document<language> document() const;

    private:
        std::string _id;
        std::string _name;
        std::string _tag;
    };
}
