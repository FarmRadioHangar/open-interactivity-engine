///
/// \file document.h
///
#pragma once

#include <bsoncxx/builder/basic/document.hpp>
#include <bsoncxx/document/value.hpp>
#include <bsoncxx/json.hpp>
#include <bsoncxx/oid.hpp>
#include <iostream>
#include <mongocxx/client.hpp>
#include <mongocxx/collection.hpp>
#include <sstream>
#include "pool.h"

namespace ops
{
    namespace mongodb
    {
        using bsoncxx::builder::basic::kvp;
        using bsoncxx::builder::basic::make_document;

        template <typename T>
        class document
        {
        public:
            document();
            explicit document(bsoncxx::document::view view);

            virtual ~document() = default;

            void fetch();
            void save();
            void remove();

            void inject(bsoncxx::document::view view);
            bsoncxx::document::view view() const;

            static void create(bsoncxx::document::view view);

            static document find(bsoncxx::document::view filter);

            template <typename K, typename V>
            static document find(const K& k, const V& v);

            static std::int64_t count();

            static std::int64_t count(
                bsoncxx::document::view filter,
                mongocxx::options::count options = mongocxx::options::count{});

            std::istringstream stream() const;

        private:
            bsoncxx::oid             _oid;
            bsoncxx::document::value _value;
        };

        template <typename T>
        document<T>::document()
          : _value{make_document(kvp("_id", _oid))}
        {
        }

        template <typename T>
        document<T>::document(bsoncxx::document::view view)
          : _oid{view["_id"].get_oid().value},
            _value{view}
        {
        }

        template <typename T> void document<T>::fetch()
        {
            auto collection = pool::instance().database().collection(T::name);

            const auto filter = make_document(kvp("_id", _oid));
            const auto result = collection.find_one(filter.view());
 
            if (!result) {
                throw std::runtime_error{"not found"};
            }
 
            _value = std::move(result.value());
        }

        template <typename T> void document<T>::save()
        {
            auto collection = pool::instance().database().collection(T::name);

            const auto filter = make_document(kvp("_id", _oid));

            mongocxx::options::update options{};
            options.upsert(true);
 
            collection.replace_one(filter.view(), _value.view(), options);
        }

        template <typename T> void document<T>::remove()
        {
            auto collection = pool::instance().database().collection(T::name);

            const auto filter = make_document(kvp("_id", _oid));

            collection.delete_one(filter.view());
 
            _oid = bsoncxx::oid{};
            _value = make_document(kvp("_id", _oid));
        }

        template <typename T>
        void document<T>::inject(bsoncxx::document::view view)
        {
            _value = bsoncxx::document::value{view};
        }

        template <typename T>
        bsoncxx::document::view document<T>::view() const
        {
            return _value.view();
        }

        template <typename T>
        void document<T>::create(bsoncxx::document::view view)
        {
            document<T> doc{};
            doc.inject(view);
            doc.save();
        }

        template <typename T>
        document<T> document<T>::find(bsoncxx::document::view filter)
        {
            auto collection = pool::instance().database().collection(T::name);

            const auto result = collection.find_one(filter);

            if (!result) {
                throw std::runtime_error{"not found"};
            }

            return document{result.value().view()};
        }

        template <typename T>
        template <typename K, typename V>
        document<T> document<T>::find(const K& k, const V& v)
        {
            return document<T>::find(make_document(kvp(k, v)));
        }

        template <typename T>
        std::int64_t document<T>::count()
        {
            auto collection = pool::instance().database().collection(T::name);
            return collection.count({});
        }

        template <typename T>
        std::int64_t document<T>::count(bsoncxx::document::view filter, 
                                        mongocxx::options::count options)
        {
            auto collection = pool::instance().database().collection(T::name);
            return collection.count(filter, options);
        }

        template <typename T>
        std::istringstream document<T>::stream() const
        {
            std::istringstream stream;
            stream.str(bsoncxx::to_json(_value.view()));

            return stream;
        }

        template <typename T>
        std::ostream& operator<<(std::ostream& os, const document<T>& doc)
        {
            os << bsoncxx::to_json(doc.view());
            return os;
        }
    }
}
