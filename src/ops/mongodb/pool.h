///
/// \file pool.h
///
#pragma once

#include <mongocxx/database.hpp>
#include <mongocxx/pool.hpp>
#include <string>

namespace ops
{
    namespace mongodb
    {
        class pool
        {
        public:
            static pool& instance();

            mongocxx::pool::entry acquire_entry() const;
            mongocxx::database database() const;

            static void init(
                const std::string& db,
                const std::string& uri = "mongodb://localhost:27017");

        private:
            pool();

            static bool                     _initialized;
            static std::string              _uri;
            static std::string              _database;
            std::shared_ptr<mongocxx::pool> _pool;
        };

        inline mongocxx::pool::entry pool::acquire_entry() const
        {
            return _pool->acquire();
        }
    }
}
