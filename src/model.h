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

/*!
 * \brief This is the main namespace for this library.
 */
namespace survey
{
    using bsoncxx::builder::basic::kvp;

    /*!
     * \class page
     *
     * \brief A \a page is a subset of documents drawn from a MongoDB collection.
     */
    template <typename T, typename Container = std::vector<T>> class page
    {
    public:
        page(Container&& collection, const std::size_t total);

        std::size_t count() const;
        std::size_t total() const;

        T at(std::size_t pos) const;

        web::json::value to_json() const;

    private:
        Container  _collection;
        std::size_t _total;
    };

    /*!
     * \brief Default constructor
     *
     * Typically, you would use survey::model::page to create a page, rather
     * than invoking this constructor directly.
     *
     * \param collection an STL collection of survey::model documents
     * \param total      the total number of documents available in the
     *                   underlying MongoDB data store at the time this
     *                   subset was generated
     */
    template <typename T, typename Container>
    page<T, Container>::page(Container&& collection, const std::size_t total)
      : _collection{std::move(collection)},
        _total{total}
    {
        assert(total >= collection.size());
    }

    /*!
     * \brief The size of this page
     *
     * \returns the number of documents in this collection. This number is
     *          often less than the number of documents available in the
     *          underlying MongoDB data store.
     *
     * \sa total
     */
    template <typename T, typename Container>
    std::size_t page<T, Container>::count() const
    {
        return _collection.size();
    }

    /*!
     * \brief The MongoDB collection's total size
     *
     * \returns the \a total number of documents available in the MongoDB
     *          collection at the time this subset was generated.
     */
    template <typename T, typename Container>
    std::size_t page<T, Container>::total() const
    {
        return _total;
    }

    /*!
     * \brief Obtain a document from this page.
     *
     * \param pos position of the element to return
     *
     * \returns the survey::model document at the given position in the
     *          collection
     */
    template <typename T, typename Container>
    T page<T, Container>::at(std::size_t pos) const
    {
        return _collection.at(pos);
    }

    /*!
     * \brief Create and return a JSON object representation of the collection.
     *
     * \code{.cpp}
     *     auto collection = pants::page(0, 10);
     *     std::cout << collection.to_json().serialize() << std::endl;
     * \endcode
     *
     * Example output:
     *
     * \code{.json}
     *     {
     *         "pants": [
     *             { "type": "party" },
     *             { "type": "fancy" },
     *             { "type": "smart" }
     *         ],
     *         "count": 10,
     *         "total": 300
     *     }
     * \endcode
     *
     * \returns a JSON object holding the array of documents
     */
    template <typename T, typename Container>
    web::json::value page<T, Container>::to_json() const
    {
        std::vector<web::json::value> values;
        for (const auto& value : _collection) {
            values.emplace_back(value.to_json());
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
     * \code{.cpp}
     *     class pants : public survey::model<pants>
     *     {
     *     public:
     *         COLLECTION(pants)
     *
     *         pants() : survey::model<pants>{"test"}
     *         {
     *         }
     *     };
     *
     *     int main()
     *     {
     *         mongocxx::instance instance{};
     *         pants document = pants::get("5b63f486be9ca51a9a3c0e81");
     *
     *         // ...
     *
     *         return 0;
     *     }
     * \endcode
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

        void set_data(bsoncxx::document::view view);
        void set_data(const std::string& data);

        bsoncxx::document::view data() const;

        web::json::value to_json() const;

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

        bsoncxx::document::element operator[](const std::string& key);

    protected:
        mongocxx::collection get_mongodb_collection() const;

    private:
        std::string              _host;
        std::string              _db;
        std::string              _collection;
        bsoncxx::document::value _data;
        bsoncxx::oid             _oid;
        mongocxx::client         _client;
    };

    /*!
     * \brief Create a MongoDB document linked to a database and a collection.
     *
     * \param uri        MongoDB host uri
     * \param db         MongoDB database name
     * \param collection MongoDB collection name
     */
    template <typename T>
    model<T>::model(mongocxx::uri&& uri, const std::string& db, const std::string& collection)
      : _host{uri.to_string()},
        _db{db},
        _collection{collection},
        _data{bsoncxx::builder::basic::make_document()},
        _client{uri}
    {
    }

    /*!
     * \brief Create a MongoDB document linked to a database and a collection
     *        using the default host uri (mongodb://localhost:27017).
     *
     * \param db         MongoDB database name
     * \param collection MongoDB collection name
     */
    template <typename T>
    model<T>::model(const std::string& db, const std::string& collection)
      : _host{"mongodb://localhost:27017"},
        _db{db},
        _collection{collection},
        _data{bsoncxx::builder::basic::make_document()},
        _client{mongocxx::uri{_host}}
    {
    }

    /*!
     * \brief Create a copy of a MongoDB document.
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
     * \brief MongoDB host uri
     *
     * \return the MongoDB host uri
     */
    template <typename T> std::string model<T>::host() const
    {
        return _host;
    }

    /*!
     * \brief MongoDB database name
     *
     * \return the name of the MongoDB database this document is linked to
     */
    template <typename T> std::string model<T>::db() const
    {
        return _db;
    }

    /*!
     * \brief MongoDB collection name
     *
     * \return the name of the MongoDB collection this document is linked to
     */
    template <typename T> std::string model<T>::collection() const
    {
        return _collection;
    }

    /*!
     * \brief Set the document's data.
     *
     * \param view a view of a document
     */
    template <typename T> void model<T>::set_data(bsoncxx::document::view view)
    {
        _data = bsoncxx::document::value{view};
    }

    /*!
     * \brief Set the document's data.
     *
     * \param data a serialized JSON object
     */
    template <typename T> void model<T>::set_data(const std::string& data)
    {
        try {
            _data = bsoncxx::from_json(data);
        } catch (bsoncxx::v_noabi::exception&) {
            throw model_error{model_error::bad_bson_data, "bad BSON data"};
        }
    }

    /*!
     * \brief Return a view of the document's data.
     */
    template <typename T> bsoncxx::document::view model<T>::data() const
    {
        return _data;
    }

    /*!
     * \brief Return the document's data serialized to a string.
     */
    template <typename T> web::json::value model<T>::to_json() const
    {
        return web::json::value::parse(bsoncxx::to_json(_data));
    }

    /*!
     * \brief Set the document's ObjectId.
     *
     * \param oid a valid MongoDB ObjectId
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

        set_data(result.value());
    }

    /*!
     * \brief Persist the document to the database. This method can be used to:
     *          * update an existing document; or to
     *          * create a new document.
     *
     * \code{.cpp}
     *     // Update a document
     *     auto document = pants::get("5b63f486be9ca51a9a3c0e81");
     *     document.set_data(web::json::value::parse("{\"key\":\"val\"}"));
     *     document.save();
     *
     *     // Create a new document
     *     pants new_document{};
     *     new_document.save();
     * \endcode
     */
    template <typename T> void model<T>::save()
    {
        auto filter = bsoncxx::builder::basic::make_document(kvp("_id", _oid));

        mongocxx::options::update options{};
        options.upsert(true);

        get_mongodb_collection().replace_one(filter.view(), _data.view(), options);

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
        _data = bsoncxx::builder::basic::make_document();
    }

    /*!
     * \brief Get the MongoDB document corresponding to the provided ObjectId.
     *
     * \param oid a valid MongoDB ObjectId string
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
     * \brief Obtain a subset of documents from a MongoDB collection.
     *
     * \param skip  offset from where MongoDB begins returning results
     * \param limit the maximum number of documents to return
     *
     * \return a STL container with a collection of documents
     *
     * \sa survey::page
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

        auto collection = model.get_mongodb_collection();
        auto cursor = collection.find({}, opts);

        Container<T, Allocator<T>> container{};

        for (auto&& doc : cursor)
        {
            T document{};
            document._oid  = doc["_id"].get_oid().value;
            document.set_data(doc);
            container.emplace_back(document);
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

    /*!
     * \brief Directly access the element of the document with the given key.
     *
     * \param key the key to match
     */
    template <typename T> bsoncxx::document::element model<T>::operator[](const std::string& key)
    {
        return _data.view()[key];
    }

    template <typename T> mongocxx::collection model<T>::get_mongodb_collection() const
    {
        return _client.database(_db).collection(_collection);
    }
}
