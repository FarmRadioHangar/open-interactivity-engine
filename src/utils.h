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
            struct property
            {
                std::string name;
                web::json::value::value_type type;
            };

            template <typename T> class document
            {
            public:
                document(web::json::value json);
                document(const std::string& data);

                void add_required_property(const std::string& name, web::json::value::value_type type);
                T build();

            private:
                void append(bsoncxx::builder::basic::document& builder, const std::string& key, const web::json::value json);
                void append(bsoncxx::builder::basic::array& builder, const web::json::value json);

                web::json::value      _json;
                std::vector<property> _properties;
            };

            template <typename T> document<T>::document(web::json::value json)
              : _json{json} {}

            template <typename T> document<T>::document(const std::string& data)
              : _json{web::json::value::parse(data)} {}

            template <typename T>
            void document<T>::add_required_property(const std::string& name,
                                                    web::json::value::value_type type)
            {
                _properties.push_back({name, type});
            }

            template <typename T>
            T document<T>::build()
            {
                bsoncxx::builder::basic::document builder{};
                T document;

                if (!_json.is_object()) {
                    throw std::runtime_error{"request is not a json object"};
                }

                for (auto& property : _properties) {
                    if (!_json.has_field(property.name)) {
                        throw std::runtime_error{"missing property"};
                    }
                    auto val = _json[property.name];
                    if (val.type() != property.type) {
                        throw std::runtime_error{"type mismatch"};
                    }
                    append(builder, property.name, val);
                }

                document.set_data(bsoncxx::to_json(builder.extract()));
                return document;
            }

            template <typename T> void document<T>::append(
                bsoncxx::builder::basic::document& builder,
                const std::string& key,
                const web::json::value json)
            {
                switch (json.type())
                {
                case web::json::value::value_type::Number:
                    builder.append(
                        kvp(key, json.is_integer() ? json.as_integer()
                                                   : json.as_double()));
                    break;
                case web::json::value::value_type::Boolean:
                    builder.append(kvp(key, json.as_bool()));
                    break;
                case web::json::value::value_type::String:
                    builder.append(kvp(key, json.as_string()));
                    break;
                case web::json::value::value_type::Object:
                {
                    bsoncxx::builder::basic::document val{};
                    for (auto& doc : json.as_object()) {
                        append(val, doc.first, doc.second);
                    }
                    builder.append(kvp(key, val));
                    break;
                }
                case web::json::value::value_type::Array:
                {
                    bsoncxx::builder::basic::array docs{};
                    for (auto& doc : json.as_array()) {
                        append(docs, doc);
                    }
                    builder.append(kvp(key, docs));
                    break;
                }
                case web::json::value::value_type::Null:
                    builder.append(
                        kvp(key, bsoncxx::builder::basic::make_document()));
                default:
                    ;
                }
            }

            template <typename T> void document<T>::append(
                bsoncxx::builder::basic::array& builder,
                const web::json::value json)
            {
                switch (json.type())
                {
                case web::json::value::value_type::Number:
                    builder.append(json.is_integer() ? json.as_integer()
                                                     : json.as_double());
                    break;
                case web::json::value::value_type::Boolean:
                    builder.append(json.as_bool());
                    break;
                case web::json::value::value_type::String:
                    builder.append(json.as_string());
                    break;
                case web::json::value::value_type::Object:
                {
                    bsoncxx::builder::basic::document val{};
                    for (auto& doc : json.as_object()) {
                        append(val, doc.first, doc.second);
                    }
                    builder.append(val);
                    break;
                }
                case web::json::value::value_type::Array:
                {
                    bsoncxx::builder::basic::array docs{};
                    for (auto& doc : json.as_array()) {
                        append(docs, doc);
                    }
                    builder.append(docs);
                    break;
                }
                case web::json::value::value_type::Null:
                    builder.append(bsoncxx::builder::basic::make_document());
                default:
                    ;
                }
            }
        }
    }
}
