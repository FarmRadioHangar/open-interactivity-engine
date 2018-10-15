///
/// \file server.h
///
#pragma once

#include <boost/regex.hpp>
#include <cpprest/http_listener.h>
#include <cstdint>
#include <string>
#include <vector>

namespace ops
{
    namespace http
    {
        class request
        {
        public:
            using handler = std::function<void(http::request)>;

            struct route
            {
                web::http::method method;
                boost::regex      pattern;
                request::handler  handler;
            };

        private:
        };

        using web::http::experimental::listener::http_listener;

        class server
        {
        public:
            server(const uint16_t port       = 9080,
                   const std::string& scheme = "http",
                   const std::string& host   = "localhost",
                   const std::string& path   = "");

            server(const server&) = delete;
            server& operator=(const server&) = delete;

            void run();
            void run(const uint16_t port);

            void set_port(const uint16_t port);

        private:
            http_listener               _listener;
            uint16_t                    _port;
            std::string                 _scheme;
            std::string                 _host;
            std::string                 _path;
            std::vector<request::route> _routes;
        };
    }
}
