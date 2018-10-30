///
/// \file model
///
#pragma once

#include <bsoncxx/document/value.hpp>
#include "../../ops/mongodb/document.h"

namespace ops
{
namespace mongodb
{
    template <typename T>
    class model
    {
    public:
        model() = default;
        virtual ~model() = default;

        bsoncxx::builder::basic::document builder() const;
        ops::mongodb::document<T> document() const;

    private:
        virtual bsoncxx::builder::basic::document get_builder() const = 0;
    };

    template <typename T>
    bsoncxx::builder::basic::document model<T>::builder() const
    {
        return get_builder();
    }

    template <typename T>
    ops::mongodb::document<T> model<T>::document() const
    {
        ops::mongodb::document<T> document{};
        document.inject(builder().extract());

        return document;
    }
}
}
