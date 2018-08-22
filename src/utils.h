/*!
 * \file utils.h
 */
#pragma once

#include <cpprest/json.h>
#include <bsoncxx/json.hpp>

namespace polls
{
    namespace utils
    {
        namespace builder
        {
            template <typename T> class document
            {
            public:
                document(const web::json::value& json);
                document(const std::string& data);

                ~document() = default;
                document(const document&) = delete;
                document& operator=(const document&) = delete;

                void add_required_property(const std::string& name,
                    web::json::value::value_type type);

                T build();

            protected:
                bsoncxx::types::value to_bson_value(const web::json::value& json);

            private:
                using prop_map = std::map<std::string, web::json::value::value_type>;

                web::json::value _json;
                prop_map         _properties;
            };

            template <typename T>
            document<T>::document(const web::json::value& json)
              : _json{json}
            {
            }

            template <typename T>
            document<T>::document(const std::string& data)
              : _json{web::json::value::parse(data)}
            {
            }

            template <typename T>
            void document<T>::add_required_property(
                const std::string& name,
                web::json::value::value_type type)
            {
                _properties.insert({name, type});
            }

            template <typename T>
            T document<T>::build()
            {
                bsoncxx::builder::basic::document builder{};
                T document;

                if (!_json.is_object()) {
                    throw std::runtime_error{"json data must be an object"};
                }

                for (auto& prop : _properties) {
                    if (!_json.has_field(prop.first)) {
                        std::stringstream oss{};
                        oss << "missing property: " << prop.first;
                        throw std::runtime_error{oss.str()};
                    }
                    auto val = _json[prop.first];
                    if (val.type() != prop.second) {
                        throw std::runtime_error{"type mismatch"};
                    }
                    builder.append(kvp(prop.first, to_bson_value(prop.second)));
                }

                document.set_data(bsoncxx::to_json(builder.extract()));
                return document;
            }

            template <typename T> bsoncxx::types::value
            document<T>::to_bson_value(const web::json::value& json)
            {
                using namespace bsoncxx::types;

                switch (json.type())
                {
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
                    case web::json::value::value_type::Object:
                    {
                        bsoncxx::builder::basic::document builder{};
                        for (auto& doc : json.as_object()) {
                            builder.append(kvp(doc.first, to_bson_value(doc.second)));
                        }
                        return value{b_document{builder.extract()}};
                    }
                    case web::json::value::value_type::Array:
                    {
                        bsoncxx::builder::basic::array builder{};
                        for (auto& doc : json.as_array()) {
                            builder.append(to_bson_value(doc));
                        }
                        return value{b_array{builder.extract()}};
                    }
                    case web::json::value::value_type::Null:
                    default:
                        return value{b_null{}};
                }
            }
        }
    }
}
