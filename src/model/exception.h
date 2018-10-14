/*!
 * \file exception.h
 */
#pragma once

#include <cpprest/json.h>
#include <stdexcept>

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
            bad_bson_data,
            validation_error
        };

        model_error(const model_error_type type, const std::string& message);
        model_error(const model_error_type type, const std::string& message, const web::json::value& data);

        virtual ~model_error() = default;

//        model_error(const model_error&) = delete;
//        model_error& operator=(const model_error&) = delete;

        /*!
         * \returns the type of error raised
         */
        model_error_type type() const { return _type; };

        /*!
         * \returns todo
         */
        web::json::value json_data() const { return _data; };

    private:
        const model_error_type _type;
        const web::json::value _data;
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

    inline model_error::model_error(const model_error_type type, const std::string& message, const web::json::value& data)
      : std::runtime_error{message},
        _type{type},
        _data{data}
    {
    }
}
