/*!
 * \file model.h
 */
#pragma once

#include <bsoncxx/exception/exception.hpp>
#include <bsoncxx/json.hpp>
#include <iostream>
#include <memory>
#include <mongocxx/client.hpp>
#include <mongocxx/collection.hpp>
#include <string>

namespace polls
{
    using bsoncxx::builder::basic::kvp;

    /*!
     * \class model
     *
     * \brief MongoDB data model base class
     * 
     * ```
     * #include <iostream>
     * #include <mongocxx/instance.hpp>
     * #include "model.h"
     *
     * class pants : public polls::model<pants>
     * {
     * public:
     *     pants() : polls::model<pants>{"test", "pants"}
     *     {
     *     }
     * };
     *
     * int main()
     * {
     *     mongocxx::instance instance{};
     *     pants document = pants::get("5b63f486be9ca51a9a3c0e81");
     *     
     *     // ...
     * 
     *     return 0;
     * }
     * ```
     *
     */
    template <typename T> class model
    {
    public:
        model(const std::string& db, const std::string& collection);
        model(const model& other);
        model& operator=(const model& other);

        virtual ~model() = default;

        void set_data(std::string&& data);
        std::string data() const;

        void set_oid(std::string&& oid);
        std::string oid() const;

        void fetch();
        void save();
        void remove();

        static T get(std::string&& oid);

    protected:
        mongocxx::collection collection() const;

    private:
        std::string      _db;
        std::string      _collection;
        std::string      _data;
        bsoncxx::oid     _oid;
        mongocxx::client _client;
    };

    /*!
     * \brief Create a MongoDB document linked to a database and a collection.
     *
     * \param db         MongoDB database name
     * \param collection MongoDB collection name
     */
    template <typename T>
    model<T>::model(const std::string& db, const std::string& collection)
      : _db{db},
        _collection{collection},
        _client{mongocxx::uri{}}
    {
    }

    template <typename T> model<T>::model(const model& other)
      : _db{other._db},
        _collection{other._collection},
        _data{other._data},
        _oid{other._oid},
        _client{mongocxx::uri{}}
    {
    }

    template <typename T> model<T>& model<T>::operator=(const model& other)
    {
        if (&other != this)
        {
            _db         = other._db;
            _collection = other._collection;
            _data       = other._data;
            _oid        = other._oid;
        }
        return *this;
    }

    /*!
     * \brief Set the document's data.
     *
     * \param data the string data
     */
    template <typename T> void model<T>::set_data(std::string&& data)
    {
        _data = std::move(data);
    }

    /*!
     * \brief Get the document's data (in string format).
     *
     * \return the serialized string data associated with the document
     */
    template <typename T> std::string model<T>::data() const
    {
        return _data;
    }

    /*!
     * \brief Set the ObjectId of a document.
     *
     * \param id a valid MongoDB ObjectId string
     */
    template <typename T> void model<T>::set_oid(std::string&& oid)
    {
        try {
            _oid = bsoncxx::oid{std::move(oid)};
        } catch(bsoncxx::v_noabi::exception& e) {
            throw std::runtime_error{"invalid oid"};
        }
    }

    /*!
     * \brief Get the document's ObjectId.
     *
     * \return the MongoDB ObjectId associated with the document
     */
    template <typename T> std::string model<T>::oid() const
    {
        return _oid.to_string();
    }

    /*!
     * \brief Fetch a document from the database.
     */
    template <typename T> void model<T>::fetch()
    {
        auto filter = bsoncxx::builder::basic::make_document(kvp("_id", _oid));
        auto result = collection().find_one(filter.view());

        if (!result) {
            throw std::runtime_error{"document not found"};
        }

        _data = bsoncxx::to_json(result.value());
    }

    /*!
     * \brief Persist the document to the database.
     */
    template <typename T> void model<T>::save()
    {
        std::unique_ptr<bsoncxx::document::value> data;

        try {
            data = std::make_unique<bsoncxx::document::value>(
                bsoncxx::from_json(_data)
            );
        } catch(bsoncxx::v_noabi::exception& e) {
            throw std::runtime_error{"bad BSON data"};
        }

        auto filter = bsoncxx::builder::basic::make_document(kvp("_id", _oid));

        mongocxx::options::update options{};
        options.upsert(true);

        collection().replace_one(filter.view(), data->view(), options);
    }

    /*!
     * \brief Delete the document from the database.
     */
    template <typename T> void model<T>::remove()
    {
        auto filter = bsoncxx::builder::basic::make_document(kvp("_id", _oid));
        collection().delete_one(filter.view());
    }

    /*!
     * \brief Get the MongoDB document corresponding to the given ObjectId.
     *
     * \param id a valid MongoDB ObjectId string
     */
    template <typename T> T model<T>::get(std::string&& oid)
    {
        T model{};

        model.set_oid(std::move(oid));
        model.fetch();

        return model;
    }

    template <typename T> mongocxx::collection model<T>::collection() const
    {
        return _client.database(_db).collection(_collection);
    }
}
