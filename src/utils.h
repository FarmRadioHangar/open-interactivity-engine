/*!
 * \file utils.h
 */
#pragma once

#include <cpprest/json.h>

namespace polls
{
    namespace utils
    {
        namespace builder
        {
            class document
            {
            public:
                document();
            };

            class array
            {
            public:
                array();
            };

            struct property
            {
                std::string                  name;
                web::json::value::value_type type;
            };

            template <typename T> class response
            {
            public:
                response(web::json::value json);
                response(const std::string& data);

                void required_property(const std::string& name, web::json::value::value_type type);

            private:
                web::json::value      _json;
                std::vector<property> _properties;
            };

            template <typename T> response<T>::response(web::json::value json)
              : _json{json}
            {
            }

            template <typename T> response<T>::response(const std::string& data)
              : _json{web::json::value::parse(data)}
            {
            }

            template <typename T>
            void response<T>::required_property(const std::string& name,
                                                web::json::value::value_type type)
            {
                _properties.push_back({name, type});
            }
        }
    }
}
