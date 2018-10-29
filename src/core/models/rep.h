///
/// \file rep.h
///
#pragma once

#include <nlohmann/json.hpp>
#include <optional>
#include "../../ops/mongodb/model.h"
#include "media.h"

namespace core
{
    class rep : public ops::mongodb::model<rep>
    {
    public:
        explicit rep(const nlohmann::json& j);

        std::string format() const;

    private:
        bsoncxx::document::view get_bson() const;

        std::string          _format;
        std::string          _language;
        std::optional<media> _media;
    };

    inline std::string rep::format() const
    {
        return _format;
    }
}
