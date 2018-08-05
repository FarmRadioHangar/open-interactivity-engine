/*!
 * \file server.h
 */
#pragma once

#include <server_http.hpp>

namespace polls
{
    namespace http
    {
        using HttpServer = SimpleWeb::Server<SimpleWeb::HTTP>;
        using status_code = SimpleWeb::StatusCode;

        /*!
         * \class request
         *
         * \brief HTTP request object
         */
        class request
        {
        public:
            request(std::shared_ptr<HttpServer::Request> request);

            inline std::string param(const int pos) const;

        private:
            std::shared_ptr<HttpServer::Request> _r;
        };

        inline std::string request::param(const int pos) const
        {
            return _r->path_match.str(pos); 
        }

        /*!
         * \class response
         *
         * \brief HTTP response object
         */
        class response
        {
        public:
            response(std::shared_ptr<HttpServer::Response> response);

            void send(status_code code, const std::string& body);
            void send_json(status_code code, const std::string& body);

        private:
            std::shared_ptr<HttpServer::Response> _r;
            SimpleWeb::CaseInsensitiveMultimap    _headers;
        };

        /*!
         * \enum method
         *
         * \brief Standard HTTP verbs suported by the server
         */
        enum method
        {
            GET = 0,
            POST,
            PUT,
            PATCH,
            DELETE
        };

        /*!
         * \class server
         */
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

            void on(
                const http::method& method, 
                const std::string& pattern, 
                std::function<void(http::request req, http::response res)> handler
            );

        private:
            HttpServer _server;
            uint16_t   _port;
        };
    }
}
