/*!
 * \file server.h
 */
#pragma once

#include <cinttypes>
#include <cpprest/asyncrt_utils.h>
#include <cpprest/http_listener.h>
#include <regex>
#include <string>

namespace polls
{
    namespace http
    {
        using query_params = std::map<utility::string_t, utility::string_t>;

        class request
        {
        public:
            request(const web::http::http_request& request, const std::smatch& match);

            template <typename T> T get_param(const std::string& name);
            template <typename T> T get_param(const std::string& name, T def);

        private:
            template <typename T> T type_conv(const std::string& str);

            web::http::http_request _request;
            std::smatch             _match;
            query_params            _params;
        };

        class response
        {
        public:
            explicit response(const web::http::http_request& request);

            void send();

        private:
            web::http::http_request  _request;
            web::http::http_response _response;
        };

        using request_handler = std::function<void(polls::http::request,
                                                   polls::http::response)>;

        struct request_route
        {
            web::http::method method;
            std::regex        regex;
            request_handler   handler;
        };

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
                const web::http::method& method, 
                const std::string& pattern, 
                request_handler handler
            );

        protected:
            void handle_request(web::http::http_request request);

        private:
            using http_listener = web::http::experimental::listener::http_listener;

            http_listener              _listener;
            std::string                _scheme;
            std::string                _host;
            uint16_t                   _port;
            std::string                _path;
            std::vector<request_route> _routes;
        };
    }
}
