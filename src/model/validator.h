/*!
 * \file validator.h
 */
#pragma once

#include "../model.h"

/*!
 * \brief This is the main namespace for this library.
 */
namespace survey
{
    /*!
     * \brief todo
     */
    template <typename T> class validator
    {
    public:
        virtual ~validator();

//        validator(const validator&) = delete;
//        validator& operator=(const validator&) = delete;

        void validate(const model<T>& document);

    protected:
        validator();

    private:
        virtual void do_validate(const model<T>& document) = 0;
    };

    template <typename T> void validator<T>::validate(const model<T>& document)
    {
        return do_validate(document);
    }

    template <typename T> validator<T>::~validator()
    {
    }

    template <typename T> validator<T>::validator()
    {
    }
}
