/*!
 * \file bson.h
 */
#pragma once

#include <bsoncxx/builder/basic/array.hpp>
#include <bsoncxx/builder/basic/document.hpp>
#include <bsoncxx/builder/basic/kvp.hpp>
#include <bsoncxx/json.hpp>
#include <bsoncxx/types/value.hpp>
#include <cpprest/json.h>

namespace ops
{
    /*!
     * \brief todo
     */
    namespace utils
    {
        /*!
         * \brief todo
         */
        class bson
        {
        public:
            static bsoncxx::types::value from_json(const web::json::value& json);
        };
    }
}
