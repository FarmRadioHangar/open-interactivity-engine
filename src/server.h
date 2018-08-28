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

        /*!
         * \brief HTTP request
         */
        class request
        {
        public:
            explicit request(const web::http::http_request& request, const std::smatch& match = std::smatch{});

            template <typename T> T get_query_param(const std::string& name, const T& def) const;
            std::string get_uri_param(size_t n) const;

            void with_json(std::function<void(web::json::value)> handler);
            void send_error_response(const std::string& error_message, int status = 500);
            void send_error_response(const web::json::value& body, int status = 500);

        private:
            template <typename T> T type_conv(const std::string& str) const
            {
                return T{};
            }

            web::http::http_request _request;
            std::smatch             _match;
            query_params            _params;
        };

        template <typename T>
        T request::get_query_param(const std::string& name, const T& def) const
        {
            try {
                return type_conv<T>(_params.at(name));
            } catch (std::exception&) {
                return def;
            }
        }

        template <> inline std::string request::type_conv(const std::string& str) const
        {
            return str;
        }

        template <> inline int64_t request::type_conv(const std::string& str) const
        {
            return std::stoi(str);
        }

        inline std::string request::get_uri_param(size_t n) const
        {
            return _match.str(n);
        }

        /*!
         * \brief HTTP response
         */
        class response
        {
        public:
            explicit response(const web::http::http_request& request);

            void set_body(const web::json::value& body_data);
            void set_body(const std::string& body_data);
            void set_status_code(web::http::status_code code);
            void send();

        private:
            web::http::http_request  _request;
            web::http::http_response _response;
        };

        void inline response::set_status_code(web::http::status_code code)
        {
            _response.set_status_code(code);
        }

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
             *
             * \param port the port number
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
