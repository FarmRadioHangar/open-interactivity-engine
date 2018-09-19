/*!
 * \file exception.h
 */
#pragma once

#include <stdexcept>

/*!
 * \brief This is the main namespace for this library.
 */
namespace ops
{
    /*!
     * \brief Model validation errors.
     */
    class validation_error : public std::runtime_error
    {
    public:
        validation_error(const std::string& message);
    };

    inline validation_error::validation_error(const std::string& message)
      : std::runtime_error{message}
    {
    }
}
