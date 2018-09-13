/*!
 * \file model.h
 */
#pragma once

#include <bsoncxx/builder/basic/kvp.hpp>
#include <bsoncxx/exception/exception.hpp>
#include <bsoncxx/json.hpp>
#include <bsoncxx/oid.hpp>
#include <cpprest/json.h>
#include <mongocxx/client.hpp>
#include <vector>
#include "model/exception.h"

#define COLLECTION(name) static constexpr auto mongodb_collection = #name;

namespace survey
{
    using bsoncxx::builder::basic::kvp;

    /*!
     * \class page
     *
     * \brief A subset of documents extracted from a MongoDB collection.
     */
    template <typename T, typename Collection = std::vector<T>> class page
    {
    public:
        page(Collection&& container, const std::size_t total);

        std::size_t count() const;
        std::size_t total() const;

        web::json::value to_json() const;

    private:
        Collection  _collection;
        std::size_t _total;
    };

    /*!
     * \brief Default constructor
     */
    template <typename T, typename Collection>
    page<T, Collection>::page(Collection&& collection, const std::size_t total)
      : _collection{std::move(collection)},
        _total{total}
    {
        assert(total >= collection.size());
    }

    /*!
     * \brief Return the number of documents in this collection. This number
     *        is often less than the number of documents available in the
     *        underlying MongoDB data store.
     *
     * \sa total
     */
    template <typename T, typename Collection>
    std::size_t page<T, Collection>::count() const
    {
        return _collection.size();
    }

    /*!
     * \brief The total number of documents in the MongoDB collection from
     *        where this subset was taken.
     *
     * \returns the \a total number of documents available in the MongoDB
     *          collection at the time this subset was generated.
     */
    template <typename T, typename Collection>
    std::size_t page<T, Collection>::total() const
    {
        return _total;
    }

    /*!
     * \brief Create and return a JSON object representation of the collection.
     *
     * \returns a json object holding an array of documents
     */
    template <typename T, typename Collection>
    web::json::value page<T, Collection>::to_json() const
    {
        std::vector<web::json::value> values;
        for (const auto& value : _collection) {
            values.emplace_back(web::json::value::parse(value.data()));
        }

        web::json::value obj{};
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
     * class pants : public survey::model<pants>
     * {
     * public:
     *     COLLECTION(pants)
     *
     *     pants() : survey::model<pants>{"test"}
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
        static constexpr std::int64_t default_page_limit = 60;

        model(mongocxx::uri&& uri, const std::string& db,
              const std::string& collection = T::mongodb_collection);
        explicit model(const std::string& db,
                       const std::string& collection = T::mongodb_collection);
        model(const model& other);
        model& operator=(const model& other);
        virtual ~model() = default;

        std::string host() const;
        std::string db() const;
        std::string collection() const;

        void set_data(web::json::value&& data);
        void set_data(const web::json::value& data);
        web::json::value data() const;

        void set_oid(const std::string& oid);
        std::string oid() const;

        void fetch();
        void save();
        void remove();

        static T get(const std::string& oid);

        static std::int64_t count();

        template <template <typename, typename> class Container = std::vector,
                  template <typename> class Allocator = std::allocator>
        static survey::page<T, Container<T, Allocator<T>>>
        page(std::int64_t skip = 0, std::int64_t limit = default_page_limit);

    protected:
        mongocxx::collection get_mongodb_collection() const;

    private:
        std::string      _host;
        std::string      _db;
        std::string      _collection;
        web::json::value _data;
        bsoncxx::oid     _oid;
        mongocxx::client _client;
    };

    /*!
     * \brief Create a MongoDB document linked to a database and a collection.
     *
     * \param host       - MongoDB host uri
     * \param db         - MongoDB database name
     * \param collection - MongoDB collection name
     */
    template <typename T>
    model<T>::model(mongocxx::uri&& uri, const std::string& db, const std::string& collection)
      : _host{uri.to_string()},
        _db{db},
        _collection{collection},
        _data{web::json::value::object()},
        _client{uri}
    {
    }

    /*!
     * \brief Create a MongoDB document linked to a database and a collection.
     *
     * \param db         - MongoDB database name
     * \param collection - MongoDB collection name
     */
    template <typename T>
    model<T>::model(const std::string& db, const std::string& collection)
      : _host{"mongodb://localhost:27017"},
        _db{db},
        _collection{collection},
        _data{web::json::value::object()},
        _client{mongocxx::uri{_host}}
    {
    }

    /*!
     * \brief Copy construct a MongoDB document.
     */
    template <typename T> model<T>::model(const model& other)
      : _host{other._host},
        _db{other._db},
        _collection{other._collection},
        _data{other._data},
        _oid{other._oid},
        _client{mongocxx::uri{_host}}
    {
    }

    /*!
     * \brief Copy assign to a MongoDB document.
     */
    template <typename T> model<T>& model<T>::operator=(const model& other)
    {
        if (&other != this)
        {
            _host       = other._host;
            _db         = other._db;
            _collection = other._collection;
            _data       = other._data;
            _oid        = other._oid;
        }
        return *this;
    }

    /*!
     * \return the MongoDB host uri
     */
    template <typename T> std::string model<T>::host() const
    {
        return _host;
    }

    /*!
     * \return the name of the MongoDB database this document is linked to
     */
    template <typename T> std::string model<T>::db() const
    {
        return _db;
    }

    /*!
     * \return the name of the MongoDB collection this document is linked to
     */
    template <typename T> std::string model<T>::collection() const
    {
        return _collection;
    }

    /*!
     * \brief Transfer ownership of the provided JSON value to the document.
     *
     * \param data - a JSON object
     */
    template <typename T> void model<T>::set_data(web::json::value&& data)
    {
        if (!data.is_object()) {
            throw model_error{model_error::bad_bson_data, "JSON value must be an object"};
        }

        _data = std::move(data);
    }

    /*!
     * \brief Set the document's data.
     *
     * \param data - a JSON object
     */
    template <typename T> void model<T>::set_data(const web::json::value& data)
    {
        if (!data.is_object()) {
            throw model_error{model_error::bad_bson_data, "JSON value must be an object"};
        }

        _data = data;
    }

    /*!
     * \brief Get the document's data.
     *
     * \return todo
     */
    template <typename T> web::json::value model<T>::data() const
    {
        return _data;
    }

    /*!
     * \brief Set the document's ObjectId.
     *
     * \param oid - a valid MongoDB ObjectId
     */
    template <typename T> void model<T>::set_oid(const std::string& oid)
    {
        try {
            _oid = bsoncxx::oid{std::move(oid)};
        } catch (bsoncxx::v_noabi::exception&) {
            throw model_error{model_error::bad_oid, "invalid ObjectId"};
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
     * \brief Fetch the document's data from the database.
     */
    template <typename T> void model<T>::fetch()
    {
        auto filter = bsoncxx::builder::basic::make_document(kvp("_id", _oid));
        auto result = get_mongodb_collection().find_one(filter.view());

        if (!result) {
            throw model_error{model_error::document_not_found, "not found"};
        }

        _data = web::json::value::parse(bsoncxx::to_json(result.value()));
    }

    /*!
     * \brief Persist the document to the database. This method can be used
     *        either to update an existing document or to create a new one.
     */
    template <typename T> void model<T>::save()
    {
        std::unique_ptr<bsoncxx::document::value> data;

        if (_data.is_null()) {
            throw model_error{model_error::empty_document, "empty document"};
        }

        try {
            data = std::make_unique<bsoncxx::document::value>(bsoncxx::from_json(_data.serialize()));
        } catch (bsoncxx::v_noabi::exception&) {
            throw model_error{model_error::bad_bson_data, "bad BSON data"};
        }

        auto filter = bsoncxx::builder::basic::make_document(kvp("_id", _oid));

        mongocxx::options::update options{};
        options.upsert(true);

        get_mongodb_collection().replace_one(filter.view(), data->view(), options);

        fetch();
    }

    /*!
     * \brief Delete the document from the database. This leaves the object in
     *        a fresh (newly constructed) state.
     */
    template <typename T> void model<T>::remove()
    {
        auto filter = bsoncxx::builder::basic::make_document(kvp("_id", _oid));

        get_mongodb_collection().delete_one(filter.view());

        _oid  = bsoncxx::oid{};
        _data = web::json::value{};
    }

    /*!
     * \brief Get the MongoDB document corresponding to the provided ObjectId.
     *
     * \param id - a valid MongoDB ObjectId string
     *
     * \return a document
     */
    template <typename T> T model<T>::get(const std::string& oid)
    {
        T document{};

        document.set_oid(oid);
        document.fetch();

        return document;
    }

    /*!
     * \brief Get a subset of documents from a MongoDB collection.
     *
     * \param skip  - offset from where MongoDB begins returning results
     * \param limit - the maximum number of documents to return
     *
     * \return a STL container with a collection of documents
     */
    template <typename T>
    template <template <typename, typename> class Container,
              template <typename> class Allocator>
    survey::page<T, Container<T, Allocator<T>>>
    model<T>::page(std::int64_t skip, std::int64_t limit)
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

        return survey::page<T, Container<T, Allocator<T>>>{
            std::move(container),
            static_cast<std::size_t>(collection.count({}))
        };
    }

    /*!
     * \brief Return the \a total number of documents available in the MongoDB
     *        collection to which this model is linked.
     */
    template <typename T> std::int64_t model<T>::count()
    {
        return T{}.get_mongodb_collection().count({});
    }

    template <typename T> mongocxx::collection model<T>::get_mongodb_collection() const
    {
        return _client.database(_db).collection(_collection);
    }
}

//namespace polls
//{
//    using bsoncxx::builder::basic::kvp;
//
//    /*!
//     * \class collection
//     *
//     * \brief A subset of documents extracted from a MongoDB collection.
//     *
//     * \todo rename to "page"?
//     */
//    template <typename T, typename Collection = std::vector<T>>
//    class collection
//    {
//    public:
//        collection(Collection&& container, const std::size_t total);
//
//        std::size_t count() const;
//        std::size_t total() const;
//
//        web::json::value to_json() const;
//
//    private:
//        Collection  _collection;
//        std::size_t _total;
//    };
//
//    /*!
//     * \brief Default constructor
//     */
//    template <typename T, typename Collection>
//    collection<T, Collection>::collection(Collection&& collection, const std::size_t total)
//      : _collection{std::move(collection)},
//        _total{total}
//    {
//        assert(total >= collection.size());
//    }
//
//    /*!
//     * \brief Return the number of documents in this collection. This number
//     * is often less than the number of documents available in the underlying
//     * MongoDB data store.
//     *
//     * \sa total
//     */
//    template <typename T, typename Collection>
//    std::size_t collection<T, Collection>::count() const
//    {
//        return _collection.size();
//    }
//
//    /*!
//     * \brief The total number of documents in the MongoDB collection from
//     * where this subset originates.
//     *
//     * \returns the \a total number of documents available in the MongoDB
//     * collection accommodating this subset, at the time it was generated.
//     */
//    template <typename T, typename Collection>
//    std::size_t collection<T, Collection>::total() const
//    {
//        return _total;
//    }
//
//    /*!
//     * \brief Create and return a JSON object representation of the collection.
//     *
//     * \returns a json object holding an array of documents
//     */
//    template <typename T, typename Collection>
//    web::json::value collection<T, Collection>::to_json() const
//    {
//        std::vector<web::json::value> values;
//        for (auto& value : _collection) {
//            values.push_back(web::json::value::parse(value.data()));
//        }
//        web::json::value obj{};
//        obj[T::mongodb_collection] = web::json::value::array(values);
//        obj["count"] = count();
//        obj["total"] = total();
//        return obj;
//    }
//
//    /*!
//     * \class model
//     *
//     * \brief MongoDB data model base class
//     *
//     * ```
//     * class pants : public polls::model<pants>
//     * {
//     * public:
//     *     COLLECTION(pants)
//     *
//     *     pants() : polls::model<pants>{"test", pants::mongodb_collection}
//     *     {
//     *     }
//     * };
//     *
//     * int main()
//     * {
//     *     mongocxx::instance instance{};
//     *     pants document = pants::get("5b63f486be9ca51a9a3c0e81");
//     *
//     *     // ...
//     *
//     *     return 0;
//     * }
//     * ```
//     */
//    template <typename T> class model
//    {
//    public:
//        static constexpr std::int64_t default_page_limit = 60;
//
//        model();
//        model(const std::string& db, const std::string& collection);
//        model(const model& other);
//        model& operator=(const model& other);
//
//        virtual ~model() = default;
//
//        void set_data(std::string&& data);
//        std::string data() const;
//
//        void set_oid(const std::string& oid);
//        std::string oid() const;
//
//        void fetch();
//        void save();
//        void remove();
//
//        static T get(const std::string& oid);
//
//        template <template <typename, typename> class Container = std::vector,
//                  template <typename> class Allocator = std::allocator>
//        static polls::collection<T, Container<T, Allocator<T>>>
//        all(std::int64_t skip = 0, std::int64_t limit = default_page_limit);
//
//        static std::int64_t count();
//        static std::int64_t count(
//            bsoncxx::document::view_or_value filter,
//            mongocxx::options::count options = mongocxx::options::count{}
//        );
//
//        static void create_index(
//            bsoncxx::document::view_or_value keys,
//            mongocxx::options::index& options = mongocxx::options::index{}
//        );
//
//    protected:
//        mongocxx::collection collection() const;
//
//    private:
//        std::string      _db;
//        std::string      _collection;
//        std::string      _data;
//        bsoncxx::oid     _oid;
//        mongocxx::client _client;
//    };
//
//    /*!
//     * \brief Default constructor
//     */
//    template <typename T> model<T>::model() : _client{mongocxx::uri{}}
//    {
//    }
//
//    /*!
//     * \brief Create a MongoDB document linked to a database and a collection.
//     *
//     * \param db         MongoDB database name
//     * \param collection MongoDB collection name
//     */
//    template <typename T>
//    model<T>::model(const std::string& db, const std::string& collection)
//      : _db{db},
//        _collection{collection},
//        _client{mongocxx::uri{}}
//    {
//    }
//
//    /*!
//     * \brief Copy construct a MongoDB document.
//     */
//    template <typename T> model<T>::model(const model& other)
//      : _db{other._db},
//        _collection{other._collection},
//        _data{other._data},
//        _oid{other._oid},
//        _client{mongocxx::uri{}}
//    {
//    }
//
//    /*!
//     * \brief Copy assign to a MongoDB document.
//     */
//    template <typename T> model<T>& model<T>::operator=(const model& other)
//    {
//        if (&other != this)
//        {
//            _db         = other._db;
//            _collection = other._collection;
//            _data       = other._data;
//            _oid        = other._oid;
//        }
//        return *this;
//    }
//
//    /*!
//     * \brief Set the document's data.
//     *
//     * \param data the string data
//     *
//     * \todo store data internally in json format instead?
//     */
//    template <typename T> void model<T>::set_data(std::string&& data)
//    {
//        _data = std::move(data);
//    }
//
//    /*!
//     * \brief Get the document's data (as a string).
//     *
//     * \return the document's data serialized to a string
//     */
//    template <typename T> std::string model<T>::data() const
//    {
//        return _data;
//    }
//
//    /*!
//     * \brief Set the document's ObjectId.
//     *
//     * \param oid a valid MongoDB ObjectId string
//     */
//    template <typename T> void model<T>::set_oid(const std::string& oid)
//    {
//        try {
//            _oid = bsoncxx::oid{std::move(oid)};
//        } catch (bsoncxx::v_noabi::exception&) {
//            throw model_error{model_error::bad_oid, "invalid ObjectId"};
//        }
//    }
//
//    /*!
//     * \brief Get the document's ObjectId.
//     *
//     * \return the MongoDB ObjectId associated with the document
//     */
//    template <typename T> std::string model<T>::oid() const
//    {
//        return _oid.to_string();
//    }
//
//    /*!
//     * \brief Fetch a document from the database.
//     */
//    template <typename T> void model<T>::fetch()
//    {
//        auto filter = bsoncxx::builder::basic::make_document(kvp("_id", _oid));
//        auto result = collection().find_one(filter.view());
//
//        if (!result) {
//            std::cout << bsoncxx::to_json(filter.view()) << std::endl;
//            throw model_error{model_error::document_not_found, "Not found."};
//        }
//
//        _data = bsoncxx::to_json(result.value());
//    }
//
//    /*!
//     * \brief Persist the document to the database. This method can both be
//     * used to update an existing document, and to create a new one.
//     */
//    template <typename T> void model<T>::save()
//    {
//        std::unique_ptr<bsoncxx::document::value> data;
//
//        if (_data.empty()) {
//            throw model_error{model_error::empty_document, "Empty document."};
//        }
//
//        try {
//            data = std::make_unique<bsoncxx::document::value>(
//                bsoncxx::from_json(_data)
//            );
//        } catch (bsoncxx::v_noabi::exception&) {
//            throw model_error{model_error::bad_bson, "Bad BSON data."};
//        }
//
//        auto filter = bsoncxx::builder::basic::make_document(kvp("_id", _oid));
//
//        mongocxx::options::update options{};
//        options.upsert(true);
//
//        std::cout << bsoncxx::to_json(filter.view()) << std::endl;
//        std::cout << bsoncxx::to_json(data->view()) << std::endl;
//
//        collection().replace_one(filter.view(), data->view(), options);
//
//        fetch();
//    }
//
//    /*!
//     * \brief Delete the document from the database. This leaves the object in
//     * a fresh (newly constructed) state.
//     */
//    template <typename T> void model<T>::remove()
//    {
//        auto filter = bsoncxx::builder::basic::make_document(kvp("_id", _oid));
//        collection().delete_one(filter.view());
//
//        _oid  = bsoncxx::oid{};
//        _data = "";
//    }
//
//    /*!
//     * \brief Get the MongoDB document corresponding to the given ObjectId.
//     *
//     * \param id a valid MongoDB ObjectId string
//     *
//     * \return a document
//     */
//    template <typename T> T model<T>::get(const std::string& oid)
//    {
//        T document{};
//
//        document.set_oid(oid);
//        document.fetch();
//
//        return document;
//    }
//
//    /*!
//     * \brief Get a subset of documents from a MongoDB collection.
//     *
//     * \param skip  offset from where MongoDB begins returning results
//     * \param limit the maximum number of documents to return
//     *
//     * \return a STL container with a collection of documents
//     *
//     * \todo rename to "page"?
//     */
//    template <typename T>
//    template <template <typename, typename> class Container,
//              template <typename> class Allocator>
//    polls::collection<T, Container<T, Allocator<T>>>
//    model<T>::all(std::int64_t skip, std::int64_t limit)
//    {
//        T model{};
//
//        mongocxx::options::find opts{};
//        opts.skip(skip);
//        opts.limit(limit);
//
//        auto collection = model.collection();
//        auto cursor = collection.find({}, opts);
//
//        Container<T, Allocator<T>> container{};
//
//        for (auto&& doc : cursor)
//        {
//            T document{};
//            document._oid  = doc["_id"].get_oid().value;
//            document._data = bsoncxx::to_json(doc);
//            container.push_back(document);
//        }
//
//        return polls::collection<T, Container<T, Allocator<T>>>{
//            std::move(container),
//            static_cast<std::size_t>(collection.count({}))
//        };
//    }
//
//    /*!
//     * \brief Return the \a total number of documents available in the MongoDB
//     * collection to which this model is linked.
//     */
//    template <typename T> std::int64_t model<T>::count()
//    {
//        return T{}.collection().count({});
//    }
//
//    /*!
//     * \brief Count the number of documents matching the provided filter in the
//     * MongoDB collection to which this model is linked.
//     */
//    template <typename T> std::int64_t model<T>::count(
//        bsoncxx::document::view_or_value filter,
//        mongocxx::options::count options)
//    {
//        return T{}.collection().count(filter, options);
//    }
//
//    /*!
//     * \brief Create an index over the collection for the provided keys with
//     * the provided options.
//     *
//     * \see https://docs.mongodb.com/manual/reference/method/db.collection.createIndex/
//     */
//    template <typename T>
//    void model<T>::create_index(
//        bsoncxx::document::view_or_value keys,
//        mongocxx::options::index& options)
//    {
//        T obj{};
//        mongocxx::collection model_collection = obj.collection();
//
//        model_collection.create_index(std::move(keys), options);
//    }
//
//    template <typename T> mongocxx::collection model<T>::collection() const
//    {
//        return _client.database(_db).collection(_collection);
//    }
//}
