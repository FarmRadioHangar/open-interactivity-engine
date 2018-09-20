/*!
 * \file error.h
 */
#pragma once

#include <cpprest/json.h>
#include <stdexcept>

namespace ops
{
    /*!
     * \brief A model data validation error.
     */
    class validation_error
    {
    public:
        validation_error(const std::string& key, const std::string& message);

        web::json::value to_json() const;

    private:
        std::string _key;
        std::string _message;
    };
}
