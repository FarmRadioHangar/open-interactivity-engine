///
/// \file page.h
///
#pragma once

#include <cassert>
#include <vector>
#include "document.h"

namespace ops
{
    namespace mongodb
    {
        ///
        /// A \a page is a subset of documents drawn from a MongoDB collection.
        ///
        template <typename T, template <typename> class Container = std::vector>
        class page
        {
            typedef typename Container<document<T>>::iterator iterator;

            page(Container<document<T>>&& collection,
                 const std::size_t offset,
                 const std::size_t size,
                 const std::size_t total);

        public:
            static constexpr std::int64_t DefaultLimit = 60;

            document<T> at(const std::size_t pos) const;

            static page<T> get(const std::int64_t skip  = 0,
                               const std::int64_t limit = DefaultLimit);

            iterator begin() noexcept;
            iterator end() noexcept;

        private:
            Container<document<T>> _collection;
            std::size_t            _offset;
            std::size_t            _size;
            std::size_t            _total;
        };

        ///
        /// \brief todo
        ///
        /// \param collection todo
        /// \param offset     todo
        /// \param size       todo
        /// \param total      todo
        ///
        template <typename T, template <typename> class Container>
        page<T, Container>::page(Container<document<T>>&& collection,
                                 const std::size_t offset,
                                 const std::size_t size,
                                 const std::size_t total)
          : _collection{std::move(collection)},
            _offset{offset},
            _size{size},
            _total{total}
        {
            assert(total >= collection.size());
        }

        ///
        /// Obtain a single document from this page.
        ///
        /// \param pos position of the element to return
        ///
        /// \returns the document at the given position in the container
        ///
        template <typename T, template <typename> class Container>
        document<T> page<T, Container>::at(const std::size_t pos) const
        {
            return _collection.at(pos);
        }

        ///
        /// \brief todo
        ///
        /// \returns todo
        ///
        template <typename T, template <typename> class Container>
        page<T> page<T, Container>::get(const std::int64_t skip,
                                        const std::int64_t limit)
        {
            mongocxx::options::find opts{};
            opts.skip(skip);
            opts.limit(limit);

            auto collection = pool::instance().database().collection(T::name);
            auto cursor = collection.find({}, opts);

            Container<document<T>> container{};

            for (const bsoncxx::document::view& bson : cursor)
                container.emplace_back(document<T>{bson});

            return page<T>{
                std::move(container), skip, limit,
                static_cast<std::size_t>(collection.count({}))};
        }

        template <typename T, template <typename> class Container>
        typename Container<document<T>>::iterator page<T, Container>::begin() noexcept
        {
            return _collection.begin();
        }

        template <typename T, template <typename> class Container>
        typename Container<document<T>>::iterator page<T, Container>::end() noexcept
        {
            return _collection.end();
        }
    }
}
