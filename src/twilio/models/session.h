///
/// \file session.h
///
#pragma once

#include <nlohmann/json.hpp>
#include "../../ops/mongodb/model.h"

namespace twilio
{
    class session : public ops::mongodb::model<session>
    {
    public:
        static auto constexpr collection = "twilioSession";

        explicit session(const nlohmann::json& j);

    private:
        bsoncxx::builder::basic::document get_builder() const;
    };
}
