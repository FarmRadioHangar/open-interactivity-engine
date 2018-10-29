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

        bsoncxx::document::view bson() const;
        ops::mongodb::document<T> document() const;

    private:
        virtual bsoncxx::document::view get_bson() const = 0;
    };

    template <typename T>
    bsoncxx::document::view model<T>::bson() const
    {
        return get_bson();
    }

    template <typename T>
    ops::mongodb::document<T> model<T>::document() const
    {
        ops::mongodb::document<T> document{};
        document.inject(bson());

        return document;
    }
}
}
