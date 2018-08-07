/*!
 * \file server.h
 */
#pragma once

#include <cinttypes>
#include <cpprest/asyncrt_utils.h>
#include <cpprest/http_listener.h>
#include <string>

namespace polls
{
    namespace http
    {
        class server
        {
        public:
            server();
            ~server() = default;
            server(const server&) = delete;
            server& operator=(const server&) = delete;

            void run();
            void run(const uint16_t port);

            /*!
             * \brief Configure the server to listen on the given port for
             * incoming connections.
             */
            void set_port(const uint16_t port) { _port = port; };

        protected:
            void handle_request(web::http::http_request request);

        private:
            using http_listener = web::http::experimental::listener::http_listener;

            http_listener _listener;
            std::string   _scheme;
            std::string   _host;
            uint16_t      _port;
            std::string   _path;
        };
    }
}
