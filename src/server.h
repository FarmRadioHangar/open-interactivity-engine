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
namespace survey
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
            request(const web::http::http_request& request, const boost::smatch& match);

            std::string get_uri_param(size_t n) const;

            template <typename T> T get_query_param(
                const std::string& name,
                const T& def) const;

            void set_status_code(web::http::status_code code);

            void send_response();
            void send_response(const web::json::value& json);
            void send_response(const std::string& data);

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

        using request_handler = std::function<void(survey::http::request)>;

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

//#include <cinttypes>
//#include <cpprest/asyncrt_utils.h>
//#include <cpprest/http_listener.h>
//#include <regex>
//#include <string>
//
//namespace survey
//{
//    namespace http
//    {
//        using query_params = std::map<utility::string_t, utility::string_t>;
//
//        class request
//        {
//        public:
//            explicit request(const web::http::http_request& request,
//                             const std::smatch& match = std::smatch{});
//
//            template <typename T> T get_query_param(const std::string& name, const T& def) const;
//            std::string get_uri_param(size_t n) const;
//
//            void with_json(std::function<void(web::json::value)> handler);
//            void send_error_response(const std::string& error_message, int status = 500);
//            void send_error_response(const web::json::value& body, int status = 500);
//
//        private:
//            template <typename T> T type_conv(const std::string& str) const
//            {
//                return T{};
//            }
//
//            web::http::http_request _request;
//            std::smatch             _match;
//            query_params            _params;
//        };
//
//        template <typename T>
//        T request::get_query_param(const std::string& name, const T& def) const
//        {
//            try {
//                return type_conv<T>(_params.at(name));
//            } catch (std::exception&) {
//                return def;
//            }
//        }
//
//        template <> inline std::string request::type_conv(const std::string& str) const
//        {
//            return str;
//        }
//
//        template <> inline int64_t request::type_conv(const std::string& str) const
//        {
//            return std::stoi(str);
//        }
//
//        inline std::string request::get_uri_param(size_t n) const
//        {
//            return _match.str(n);
//        }
//
//        class response
//        {
//        public:
//            explicit response(const web::http::http_request& request);
//
//            void set_body(const web::json::value& body_data);
//            void set_body(const std::string& body_data);
//            void set_status_code(web::http::status_code code);
//            void send();
//
//        private:
//            web::http::http_request  _request;
//            web::http::http_response _response;
//        };
//
//        void inline response::set_status_code(web::http::status_code code)
//        {
//            _response.set_status_code(code);
//        }
//
//        using request_handler = std::function<void(survey::http::request,
//                                                   survey::http::response)>;
//
//        struct request_route
//        {
//            web::http::method method;
//            std::regex        regex;
//            request_handler   handler;
//        };
//
//        class server
//        {
//        public:
//            server();
//            ~server() = default;
//            server(const server&) = delete;
//            server& operator=(const server&) = delete;
//
//            void run();
//            void run(const uint16_t port);
//
//            /*!
//             * \brief Configure the server to listen on the given port for
//             * incoming connections.
//             *
//             * \param port - the port number
//             */
//            void set_port(const uint16_t port) { _port = port; };
//
//            void on(
//                const web::http::method& method,
//                const std::string& pattern,
//                request_handler handler
//            );
//
//        protected:
//            void handle_request(web::http::http_request request);
//
//        private:
//            using http_listener = web::http::experimental::listener::http_listener;
//
//            http_listener              _listener;
//            std::string                _scheme;
//            std::string                _host;
//            uint16_t                   _port;
//            std::string                _path;
//            std::vector<request_route> _routes;
//        };
