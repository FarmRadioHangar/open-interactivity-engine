///
/// \file media.h
///
#pragma once

#include <nlohmann/json.hpp>
#include "../../ops/mongodb/model.h"

namespace core
{
    class media : public ops::mongodb::model<media>
    {
    public:
        static auto constexpr collection = "media";

        explicit media(const nlohmann::json& j);

    private:
        bsoncxx::builder::basic::document get_builder() const;

        std::optional<std::string> _id;
        std::optional<std::string> _file;
    };
}
