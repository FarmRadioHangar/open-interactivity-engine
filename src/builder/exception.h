/*!
 * \file exception.h
 */
#pragma once

#include <cpprest/json.h>
#include <stdexcept>

namespace polls
{
    namespace utils
    {
        namespace builder
        {
            enum error_type
            {
                json_not_an_object,
                missing_property,
                type_mismatch,
                unique_constraint_violation
            };

            class error : public std::runtime_error
            {
            public:
                error(const error_type type, const std::string& message, const int status = 400);
                virtual ~error() = default;

                int status_code() const { return _status; }
                std::string atom() const;

                /*!
                 * \brief todo
                 */
                web::json::value to_json() const { return to_json_impl(); }

            protected:
                virtual web::json::value to_json_impl() const;

            private:
                const error_type _type;
                const int        _status;
            };

            class key_validation_error : public error
            {
            public:
                key_validation_error(const std::string& key, const error_type type, const std::string& message, const int status = 400);
                virtual ~key_validation_error() = default;

                std::string key() const { return _key; }

            protected:
                virtual web::json::value to_json_impl() const override;

            private:
                const std::string _key;
            };
        }
    }
}
