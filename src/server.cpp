#include "server.h"
#include <sstream>

namespace polls
{
    namespace http
    {
        static constexpr const char* to_string(const method& method)
        {
            switch (method)
            {
            case POST:
                return "POST";
            case PUT:
                return "PUT";
            case PATCH:
                return "PATCH";
            case DELETE:
                return "DELETE";
            case GET:
            default:
                return "GET";
            }
        }

        request::request(std::shared_ptr<HttpServer::Request> request)
          : _r{request}
        {
        }

        response::response(std::shared_ptr<HttpServer::Response> response)
          : _r{response} 
        {
        }

        /*!
         * \brief Send generic response.
         *
         * \param code HTTP status code
         * \param body the response body
         */
        void response::send(status_code code, const std::string& body)
        {
            _headers.emplace("Content-Length", std::to_string(body.length()));
            _headers.emplace("Access-Control-Allow-Origin", "*");
            _r->write(code, body, _headers);
        }

        /*!
         * \brief Send JSON response.
         *
         * \param code HTTP status code
         * \param body the response body
         */
        void response::send_json(status_code code, const std::string& body)
        {
            _headers.emplace("Content-Type", "application/json");
            send(code, body);
        }

        /*!
         * \brief Create the server
         */
        server::server()
          : _server{},
            _port{9080}
        {
        }

        /*!
         * \brief Run the server
         */
        void server::run()
        {
            _server.config.port = _port;
            _server.start();
        }

        /*!
         * \brief Run the server on a specific port.
         */
        void server::run(const uint16_t port)
        {
            set_port(port);
            run();
        }

        /*!
         * \brief Register a request handler with the server.
         *
         * \param method HTTP method
         * \param pattern regular expression to match on
         * \param handler a request handler that will process the request
         */
        void server::on(
            const http::method& method,
            const std::string& pattern,
            polls::http::request_handler handler)
        {
            _server.resource[pattern][http::to_string(method)] = [handler](
                std::shared_ptr<HttpServer::Response> response,
                std::shared_ptr<HttpServer::Request> request)
            {
                try {
                    handler(polls::http::request{request}, polls::http::response{response});
                } catch(std::exception& e) {
                    polls::http::response res{response};
                    std::ostringstream oss{};
                    oss << "{\"message\":\"" << e.what() << "\"}";
                    res.send_json(status_code::server_error_internal_server_error, oss.str());
                    std::cout << e.what() << std::endl;
                }
            };
        }
    }
}
