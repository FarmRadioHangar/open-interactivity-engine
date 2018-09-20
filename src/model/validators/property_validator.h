/*!
 * \file property_validator.h
 */
#pragma once

#include "../validator.h"
#include <bsoncxx/types.hpp>
#include <list>
#include <map>
#include <set>
#include "error.h"

namespace ops
{
    /*!
     * \brief todo
     */
    struct prop
    {
        /*!
         * \brief todo
         */
        enum prop_type
        {
            t_string,
            t_numeric,
            t_boolean,
            t_null,
            t_array,
            t_object
        };

        std::list<prop_type> types;
        bool required;
    };

    /*!
     * \brief todo
     */
    template <typename T>
    class property_validator : public validator<T>
    {
    public:
        using prop_list = std::list<prop::prop_type>;

        property_validator();
        explicit property_validator(const std::map<std::string, prop>& map);

        void add_property(const std::string& name,
                          prop::prop_type type,
                          const bool required = false);

        void add_property(const std::string& name,
                          const std::list<prop::prop_type>& types,
                          const bool required = false);

    private:
        void do_validate(const T& document) override;

        bool validates_as(bsoncxx::type bson_type, prop::prop_type json_type) const;

        std::map<std::string, prop> _properties;
    };

    /*!
     * \brief todo
     */
    template <typename T>
    property_validator<T>::property_validator()
      : validator<T>{}
    {
    }

    /*!
     * \brief todo
     */
    template <typename T>
    property_validator<T>::property_validator(const std::map<std::string, prop>& map)
      : validator<T>{},
        _properties{map}
    {
    }

    /*!
     * \brief todo
     *
     * \param name     todo
     * \param type     todo
     * \param required whether this property is required or not
     */
    template <typename T>
    void property_validator<T>::add_property(
        const std::string& name,
        prop::prop_type type,
        const bool required)
    {
        _properties.insert({name, {
            std::list<prop::prop_type>{type}, required
        }});
    }

    /*!
     * \brief todo
     *
     * \param name     todo
     * \param types    todo
     * \param required whether this property is required or not
     */
    template <typename T>
    void property_validator<T>::add_property(
        const std::string& name,
        const std::list<prop::prop_type>& types,
        const bool required)
    {
        _properties.insert({name, {types, required}});
    }

    template <typename T>
    void property_validator<T>::do_validate(const T& document)
    {
        for (const auto& property : _properties)
        {
            const auto& key = property.first;
            const auto& val = property.second;
            const auto& field = document[key];

            if (val.required && !field) {
                validator<T>::_errors.push_back(validation_error{key,
                    "Missing property '" + key + "'"});
                continue;
            }
            if (field) {
                bool validates = false;
                for (const auto& type : val.types) {
                    if (validates_as(field.type(), type)) {
                        validates = true;
                        break;
                    }
                }
                if (!validates) {
                    validator<T>::_errors.push_back(validation_error{key,
                        "Type mismatch for key '" + key + "'"});
                }
            }
        }
    }

    template <typename T>
    bool property_validator<T>::validates_as(bsoncxx::type bson_type,
                                             prop::prop_type json_type) const
    {
        using bsoncxx::type;

        switch (json_type)
        {
            case prop::t_string:
                return (type::k_utf8 == bson_type);
            case prop::t_numeric:
                return (type::k_double == bson_type
                     || type::k_int32 == bson_type
                     || type::k_int64 == bson_type);
            case prop::t_boolean:
                return (type::k_bool == bson_type);
            case prop::t_null:
                return (type::k_null == bson_type);
            case prop::t_array:
                return (type::k_array == bson_type);
            case prop::t_object:
                return (type::k_document == bson_type);
            default:
                return false;
        }
    }
}
