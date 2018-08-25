/*!
 * \file exception.h
 */
#pragma once

#include <stdexcept>

namespace polls
{
    namespace utils
    {
        namespace builder
        {
            enum error_type {
                json_not_an_object,
                missing_property,
                type_mismatch,
                unique_constraint_violation
            };

            class error : public std::runtime_error
            {
            public:
                error(const error_type type, const std::string& message);
                virtual ~error() = default;

                error_type type() const { return _type; }

            private:
                const error_type _type;
            };

            class key_validation_error : public error
            {
            public:
                key_validation_error(const error_type type, const std::string& message, const std::string& key);
                virtual ~key_validation_error() = default;

                std::string key() const { return _key; }

            private:
                const std::string _key;
            };
        }
    }
}
