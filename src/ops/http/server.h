///
/// \file server.h
///
#pragma once

#include <boost/regex.hpp>
#include <cpprest/http_listener.h>
#include <cstdint>
#include <map>
#include <string>
#include <vector>

namespace ops
{
    namespace http
    {
        using query_params = std::map<std::string, std::string>;

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

            request(web::http::http_request&& request, const boost::smatch& match);

            std::string get_uri_param(size_t n) const;

            template <typename T>
            T get_query_param(const std::string& name,
                              const T& def) const;

            void set_status_code(web::http::status_code code);

            void with_body(std::function<void(const std::string&)> handler);

            void send_response(const std::string& body = "");
            void send_error_response(web::http::status_code code,
                                     const std::string& tag,
                                     const std::string& error);

        private:
            template <typename T> T type_conv(const std::string& str) const;

            boost::smatch            _match;
            query_params             _params;
            web::http::http_request  _request;
            web::http::http_response _response;
        };

        inline std::string request::get_uri_param(size_t n) const
        {
            return _match.str(n);
        }

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

        template <typename T>
        T request::type_conv(const std::string& str) const
        {
            return T{};
        }

        template <>
        inline std::string request::type_conv(const std::string& str) const
        {
            return str;
        }

        template <>
        inline int64_t request::type_conv(const std::string& str) const
        {
            return std::stoi(str);
        }

        using web::http::experimental::listener::http_listener;

        class server
        {
        public:
            server(const uint16_t port       = 9080,
                   const std::string& scheme = "http",
                   const std::string& host   = "localhost",
                   const std::string& path   = "");

            virtual ~server() = default;

            server(const server&) = delete;
            server& operator=(const server&) = delete;

            void run();
            void run(const uint16_t port);

            void set_port(const uint16_t port);

            void on(web::http::method method,
                    const std::string& uri_pattern,
                    request::handler handler);

        protected:
            void handle_request(web::http::http_request request);

        private:
            http_listener               _listener;
            uint16_t                    _port;
            std::string                 _scheme;
            std::string                 _host;
            std::string                 _path;
            std::vector<request::route> _routes;
        };

        inline void server::set_port(const uint16_t port)
        {
            _port = port;
        }
    }
}
