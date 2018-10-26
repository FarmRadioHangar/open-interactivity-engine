#include "pool.h"
#include <iostream>
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/pool.hpp>
#include <stdexcept>

namespace ops
{
namespace mongodb
{

static mongocxx::instance instance{};

///
/// \class pool
///
/// \brief MongoDB database connection pool
/// 
/// This is a singleton class. Call pool::init (once), passing the name of the 
/// database used in the application, to initialize the connection pool:
///
/// \code
/// int main()
/// {
///     ops::mongodb::pool::init("database");
///     // ...
/// \endcode
///
/// Then use pool::instance to access the singleton.
///
/// \code
/// auto db = ops::mongodb::pool::instance();
/// auto client = db.acquire_entry();
/// \endcode
///
/// \sa pool::instance
///

///
/// \returns the database pool singleton instance
///
mongodb::pool& pool::instance()
{
    if (!_initialized) {
        throw std::runtime_error{"pool::init has not been called"};
    }

    static mongodb::pool instance{};
    return instance;
}

///
/// \returns the database used for this application
///
mongocxx::database pool::database() const
{
    const auto client = acquire_entry();
    return client->database(_database);
}

///
/// \brief Initialize the database connection pool.
///
/// \param db  the name of the database used in this application
/// \param uri a valid MongoDB connection string
///
/// \sa https://docs.mongodb.com/manual/reference/connection-string/
///
void pool::init(const std::string& db, const std::string& uri)
{
    if (!_initialized) {
        _uri = uri;
        _database = db;
        _initialized = true;
    } else {
        std::cout << "notice: calling pool::init more than once has no effect" << std::endl;
    }
}

pool::pool() : _pool{std::make_shared<mongocxx::pool>(mongocxx::uri{_uri})}
{
}

///
/// \fn pool::acquire_entry
///
/// \brief Acquire a client from the database connection pool.
///

bool pool::_initialized = false;
std::string pool::_uri;
std::string pool::_database;

} // namespace mongodb
} // namespace ops
