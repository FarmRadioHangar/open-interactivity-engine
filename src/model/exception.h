/*!
 * \file exception.h
 */
#pragma once

#include <stdexcept>

namespace polls
{
    class model_error : public std::runtime_error
    {
    public:
        enum model_error_type
        {
            document_not_found,
            bad_oid,
            empty_document,
            bad_bson
        };

        model_error(const model_error_type type, const std::string& message);

        model_error_type type() const { return _type; };

    private:
        const model_error_type _type;
    };
}
