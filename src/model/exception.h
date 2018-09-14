/*!
 * \file exception.h
 */
#pragma once

#include <stdexcept>

namespace survey
{
    class model_error : public std::runtime_error
    {
    public:
        enum model_error_type
        {
            document_not_found,
            bad_oid,
            empty_document,
            bad_bson_data
        };

        model_error(const model_error_type type, const std::string& message);

        model_error_type type() const { return _type; };

    private:
        const model_error_type _type;
    };

    inline model_error::model_error(const model_error_type type, const std::string& message)
      : std::runtime_error{message},
        _type{type}
    {
    }
}
