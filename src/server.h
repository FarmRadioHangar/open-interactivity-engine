/*!
 * \file server.h
 */
#pragma once

#include <boost/regex.hpp>
#include <cpprest/http_listener.h>
#include <map>
#include <vector>

/*!
 * \brief This is the main namespace for this library.
 */
namespace ops
{
    /*!
     * \brief Namespace for HTTP server and utilities
     */
    namespace http
    {
        using query_params = std::map<utility::string_t, utility::string_t>;

        /*!
         * \brief A HTTP request
         */
        class request
        {
        public:
            request(web::http::http_request&& request, const boost::smatch& match);

            std::string get_uri_param(size_t n) const;

            template <typename T> T get_query_param(
                const std::string& name,
                const T& def) const;

            void set_status_code(web::http::status_code code);

            void with_json(std::function<void(web::json::value)> handler);
            void with_body(std::function<void(const std::string&)> handler);

            void send_response();
            void send_response(const web::json::value& json);
            void send_response(const std::string& data);

            void send_error_response(web::http::status_code code,
                                     const std::string& atom,
                                     const std::string& error);

        private:
            template <typename T> T type_conv(const std::string& str) const
            {
                return T{};
            }

            boost::smatch            _match;
            query_params             _params;
            web::http::http_request  _request;
            web::http::http_response _response;
        };

        /*!
         * \brief Obtain the nth uri parameter from the list of matches.
         *
         * \param n todo
         *
         * \returns todo
         */
        inline std::string request::get_uri_param(size_t n) const
        {
            return _match.str(n);
        }

        /*!
         * \brief todo
         *
         * \param name todo
         * \param def  todo
         *
         * \returns todo
         */
        template <typename T>
        T request::get_query_param(const std::string& name, const T& def) const
        {
            try {
                return type_conv<T>(_params.at(name));
            } catch (std::exception&) {
                return def;
            }
        }

        inline void request::set_status_code(web::http::status_code code)
        {
            _response.set_status_code(code);
        }

        template <> inline std::string request::type_conv(const std::string& str) const
        {
            return str;
        }

        template <> inline int64_t request::type_conv(const std::string& str) const
        {
            return std::stoi(str);
        }

        using request_handler = std::function<void(ops::http::request)>;

        /*!
         * \brief Route handler
         */
        struct request_route
        {
            web::http::method method;
            boost::regex      regex;
            request_handler   handler;
        };

        /*!
         * \brief REST server
         */
        class server
        {
        public:
            server(const uint16_t port       = 9080,
                   const std::string& scheme = "http",
                   const std::string& host   = "localhost",
                   const std::string& path   = "");

            ~server() = default;
            server(const server&) = delete;
            server& operator=(const server&) = delete;

            void run();
            void run(const uint16_t port);

            /*!
             * \brief Configure the server to listen on the given port for
             *        incoming connections.
             *
             * \param port the port number
             */
            void set_port(const uint16_t port) { _port = port; };

            void on(
                const web::http::method& method,
                const std::string& uri_pattern,
                request_handler handler);

        protected:
            void handle_request(web::http::http_request request);

        private:
            web::http::experimental::listener::http_listener _listener;
            uint16_t                   _port;
            std::string                _scheme;
            std::string                _host;
            std::string                _path;
            std::vector<request_route> _routes;
        };
    }
}
