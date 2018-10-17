///
/// \file builder.h
///
#pragma once

#include <bsoncxx/builder/basic/document.hpp>
#include <nlohmann/json.hpp>

namespace ops
{
    namespace mongodb
    {
        namespace bson
        {
            class builder : public bsoncxx::builder::basic::document
            {
            public:
                explicit builder(const nlohmann::json& j);

                bool has_errors() const;
            };
        }
    }
}
