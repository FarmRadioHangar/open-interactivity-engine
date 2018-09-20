/*!
 * \file validator.h
 */
#pragma once

#include <list>
#include "validators/error.h"

namespace ops
{
    /*!
     * \brief todo
     */
    template <typename T> class validator
    {
    public:
        virtual ~validator() = default;

        validator(const validator&) = delete;
        validator& operator=(const validator&) = delete;

        void validate(const T& document);

        std::list<validation_error> errors() const;
        
    protected:
        validator() = default;

        std::list<validation_error> _errors;

    private:
        virtual void do_validate(const T& document) = 0;
    };

    template <typename T> void validator<T>::validate(const T& document)
    {
        return do_validate(document);
    }

    template <typename T> std::list<validation_error> validator<T>::errors() const
    {
        return _errors;
    }
}
