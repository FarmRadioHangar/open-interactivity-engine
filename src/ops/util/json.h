///
/// \file json.h
///
#pragma once

#include <nlohmann/json.hpp>
#include "../mongodb/document.h"

namespace ops
{
namespace util
{
namespace json
{
    template <typename T>
    nlohmann::json extract(const mongodb::document<T>& doc)
    {
        nlohmann::json j;
        doc.stream() >> j;

        j.erase("_id");

        return j;
    }
}
}
}
