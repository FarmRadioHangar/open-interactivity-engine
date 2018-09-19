/*!
 * \file bson.h
 */
#pragma once

#include <bsoncxx/builder/basic/array.hpp>
#include <bsoncxx/builder/basic/document.hpp>
#include <bsoncxx/builder/basic/kvp.hpp>
#include <bsoncxx/json.hpp>
#include <bsoncxx/types/value.hpp>
#include <cpprest/json.h>

namespace ops
{
    class bson_utils
    {
    public:
        static bsoncxx::types::value from_json(const web::json::value& json);
    };

    bsoncxx::types::value 
    bson_utils::from_json(const web::json::value& json)
    {
        using namespace bsoncxx::types;
        using bsoncxx::builder::basic::kvp;

        switch (json.type())
        {
            case web::json::value::value_type::Object:
            {
                bsoncxx::builder::basic::document builder{};
                for (const auto& keyval : json.as_object()) {
                    builder.append(kvp(keyval.first, from_json(keyval.second)));
                }
                return value{b_document{builder.extract()}};
            }
            case web::json::value::value_type::Array:
            {
                bsoncxx::builder::basic::array builder{};
                for (const auto& doc : json.as_array()) {
                    builder.append(from_json(doc));
                }
                return value{b_array{builder.extract()}};
            }
            case web::json::value::value_type::Number:
            {
                if (json.is_integer()) {
                    return value{b_int64{json.as_integer()}};
                } else {
                    return value{b_double{json.as_double()}};
                }
            }
            case web::json::value::value_type::Boolean:
                return value{b_bool{json.as_bool()}};
            case web::json::value::value_type::String:
                return value{b_utf8{json.as_string()}};
            case web::json::value::value_type::Null:
            default:
                return value{b_null{}};
        }
    }
}
