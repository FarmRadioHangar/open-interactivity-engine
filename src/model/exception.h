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
     * \brief Runtime errors related to ops::model documents.
     */
    class model_error : public std::runtime_error
    {
    public:
        /*!
         * \brief The types of errors reprensented by this class.
         */
        enum model_error_type
        {
            document_not_found,
            bad_oid,
            bad_bson_data
        };

        model_error(const model_error_type type, const std::string& message);

        /*!
         * \returns the type of error raised
         */
        model_error_type type() const { return _type; };

    private:
        const model_error_type _type;
    };

    /*!
     * \brief Default constructor
     *
     * *Example of throwing a model_error exception:*
     *
     * \code{.cpp}
     *     throw model_error{model_error::bad_bson_data, "bad BSON"};
     * \endcode
     */
    inline model_error::model_error(const model_error_type type, const std::string& message)
      : std::runtime_error{message},
        _type{type}
    {
    }
}
