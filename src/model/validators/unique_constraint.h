/*!
 * \file unique_constraint.h
 */
#pragma once

#include "../validator.h"
#include <bsoncxx/builder/basic/document.hpp>
#include <bsoncxx/builder/basic/kvp.hpp>
#include <list>
#include "exception.h"

/*!
 * \brief This is the main namespace for this library.
 */
namespace ops
{
    /*!
     * \brief todo
     */
    template <typename T>
    class unique_constraint : public validator<T>
    {
    public:
        unique_constraint();
        unique_constraint(std::list<std::string> names);

        void add_key(const std::string& name);

    private:
        void do_validate(const T& document) override;

        std::list<std::string> _keys;
    };

    template <typename T>
    unique_constraint<T>::unique_constraint()
      : validator<T>{}
    {
    }

    template <typename T>
    unique_constraint<T>::unique_constraint(std::list<std::string> names)
      : validator<T>{},
        _keys{names}
    {
    }

    template <typename T>
    void unique_constraint<T>::do_validate(const T& document)
    {
        using bsoncxx::builder::basic::kvp;

        for (const auto& key : _keys) 
        {
            const auto& field = document[key];

            if (field) 
            {
                bsoncxx::builder::basic::document filter{};
                filter.append(kvp(key, field.get_value()));
                filter.append(kvp(
                    "_id", bsoncxx::builder::basic::make_document(kvp(
                        "$ne", bsoncxx::oid{document.oid()}))));

                if (T::count(filter.view()) > 0) {
                    throw validation_error{"Unique constraint violation for key '" + key + "'"};
                }
            }
        }
    }

    template <typename T>
    void unique_constraint<T>::add_key(const std::string& name)
    {
        _keys.push_back(name);
    }
}
