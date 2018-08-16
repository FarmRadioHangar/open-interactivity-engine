/*!
 * \file model.h
 */
#pragma once

#include <bsoncxx/exception/exception.hpp>
#include <bsoncxx/json.hpp>
#include <cpprest/asyncrt_utils.h>
#include <cpprest/http_listener.h>
#include <iostream>
#include <memory>
#include <mongocxx/client.hpp>
#include <mongocxx/collection.hpp>
#include <mongocxx/cursor.hpp>
#include <mongocxx/instance.hpp>
#include <string>

#define COLLECTION(name) static constexpr auto mongodb_collection = #name;

namespace polls
{
    using namespace web;
    using namespace web::http;
    using namespace web::http::client;

    using bsoncxx::builder::basic::kvp;

    /*!
     * \class collection
     */
    template <typename T, typename Collection = std::vector<T>>
    class collection
    {
    public:
        collection(Collection&& container, const std::int64_t total);

        std::int64_t count() const;
        std::int64_t total() const;

        json::value json() const;

    private:
        Collection   _collection;
        std::int64_t _total;
    };

    /*!
     * \brief Default constructor
     */
    template <typename T, typename Collection>
    collection<T, Collection>::collection(Collection&& collection,
                                          const std::int64_t total)
      : _collection{std::move(collection)},
        _total{total}
    {
    }

    /*!
     * \brief Return the number of documents in this collection. This number
     * may be less than the number of documents available in the underlying
     * MongoDB data store.
     *
     * \sa total
     */
    template <typename T, typename Collection>
    std::int64_t collection<T, Collection>::count() const
    {
        return _collection.size();
    }

    /*!
     * \brief Return the \a total number of documents in the underlying MongoDB
     * collection from which this subset originates.
     */
    template <typename T, typename Collection>
    std::int64_t collection<T, Collection>::total() const { return _total; }

    /*!
     * \brief Return a JSON object representation of the collection.
     */
    template <typename T, typename Collection>
    json::value collection<T, Collection>::json() const
    {
        std::vector<json::value> values;
        for (auto& value : _collection) {
            values.push_back(json::value::parse(value.data()));
        }
        json::value obj;
        obj[T::mongodb_collection] = web::json::value::array(values);
        obj["count"] = count();
        obj["total"] = total();
        return obj;
    }

    /*!
     * \class model
     *
     * \brief MongoDB data model base class
     *
     * ```
     * class pants : public polls::model<pants>
     * {
     * public:
     *     COLLECTION(pants)
     *
     *     pants() : polls::model<pants>{"test", pants::mongodb_collection}
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
     */
    template <typename T> class model
    {
    public:
        static constexpr std::int64_t max_page_limit = 60;

        model();
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

        template <template <typename, typename> class Container = std::vector,
                  template <typename> class Allocator = std::allocator>
        static polls::collection<T, Container<T, Allocator<T>>>
        all(std::int64_t skip = 0, std::int64_t limit = max_page_limit);

        static std::int64_t count();

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
     * \brief Default constructor
     */
    template <typename T> model<T>::model() : _client{mongocxx::uri{}} {}

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

    /*!
     * \brief Copy construct a MongoDB document.
     */
    template <typename T> model<T>::model(const model& other)
      : _db{other._db},
        _collection{other._collection},
        _data{other._data},
        _oid{other._oid},
        _client{mongocxx::uri{}}
    {
    }

    /*!
     * \brief Copy assign to a MongoDB document.
     */
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
     * \brief Set the document's ObjectId.
     *
     * \param id a valid MongoDB ObjectId string
     */
    template <typename T> void model<T>::set_oid(std::string&& oid)
    {
        try {
            _oid = bsoncxx::oid{std::move(oid)};
        } catch(bsoncxx::v_noabi::exception& e) {
            throw std::runtime_error{"invalid ObjectId"};
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
            std::cout << bsoncxx::to_json(filter.view()) << std::endl;
            throw std::runtime_error{"document not found"};
        }

        _data = bsoncxx::to_json(result.value());
    }

    /*!
     * \brief Persist the document to the database. This method can both be
     * used to update an existing document, and to create a new one.
     */
    template <typename T> void model<T>::save()
    {
        std::unique_ptr<bsoncxx::document::value> data;

        if (_data.empty()) {
            throw std::runtime_error{"empty (null) document"};
        }

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
     * \brief Delete the document from the database. This leaves the object in
     * a fresh (newly constructed) state.
     */
    template <typename T> void model<T>::remove()
    {
        auto filter = bsoncxx::builder::basic::make_document(kvp("_id", _oid));
        collection().delete_one(filter.view());

        _oid  = bsoncxx::oid{};
        _data = "";
    }

    /*!
     * \brief Get the MongoDB document corresponding to the given ObjectId.
     *
     * \param id a valid MongoDB ObjectId string
     *
     * \return a document
     */
    template <typename T> T model<T>::get(std::string&& oid)
    {
        T document{};

        document.set_oid(std::move(oid));
        document.fetch();

        return document;
    }

    /*!
     * \brief Get all documents from a collection.
     *
     * \param skip offset from where MongoDB begins returning results
     * \param limit the maximum number of documents to return
     *
     * \return a STL container of documents
     */
    template <typename T>
    template <template <typename, typename> class Container,
              template <typename> class Allocator>
    polls::collection<T, Container<T, Allocator<T>>>
    model<T>::all(std::int64_t skip, std::int64_t limit)
    {
        T model{};

        mongocxx::options::find opts{};
        opts.skip(skip);
        opts.limit(limit);

        auto collection = model.collection();
        auto cursor = collection.find({}, opts);

        Container<T, Allocator<T>> container{};

        for (auto&& doc : cursor)
        {
            T document{};
            document._oid  = doc["_id"].get_oid().value;
            document._data = bsoncxx::to_json(doc);
            container.push_back(document);
        }

        return polls::collection<T, Container<T, Allocator<T>>>{
            std::move(container), collection.count({})
        };
    }

    /*!
     * \brief Return the \a total number of documents available in this
     * collection.
     */
    template <typename T> std::int64_t model<T>::count()
    {
        return T{}.collection().count({});
    }

    template <typename T> mongocxx::collection model<T>::collection() const
    {
        return _client.database(_db).collection(_collection);
    }
}
