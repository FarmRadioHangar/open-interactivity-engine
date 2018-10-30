///
/// \file language.h
///
#pragma once

#include <nlohmann/json.hpp>
#include <optional>
#include "../../ops/mongodb/model.h"

namespace core
{
    class language : public ops::mongodb::model<language>
    {
    public:
        static auto constexpr collection = "languages";

        explicit language(const nlohmann::json& j);

        std::optional<std::string> id() const;
        std::string name() const;
        std::string tag() const;

    private:
        bsoncxx::builder::basic::document get_builder() const;

        std::optional<std::string> _id;
        std::string                _name;
        std::string                _tag;
    };

    inline std::optional<std::string> language::id() const
    {
        return _id;
    }

    inline std::string language::name() const
    {
        return _name;
    }

    inline std::string language::tag() const
    {
        return _tag;
    }
}
