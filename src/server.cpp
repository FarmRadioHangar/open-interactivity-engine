#include "server.h"
#include <iomanip>
#include <mongocxx/exception/query_exception.hpp>

using namespace web;
using namespace web::http;
using namespace http::experimental::listener;

namespace polls
{
    namespace http
    {
        request::request(const web::http::http_request& request, const std::smatch& match)
          : _request{request},
            _match{match},
            _params{web::uri::split_query(request.request_uri().query())}
        {
        }

        void request::with_json(std::function<void(web::json::value)> handler)
        {
            _request.extract_json().then(handler).wait();
        }

        void request::send_error_response(const std::string& error_message, int status)
        {
            std::stringstream oss{};
            oss << "{\"error\":" << std::quoted(error_message) << ",\"status\":" << status << "}";
            polls::http::response response{_request};
            response.set_status_code(status);
            response.set_body(oss.str());
            response.send();
        }

        response::response(const web::http::http_request& request)
          : _request{request},
            _response{status_codes::OK}
        {
            web::http::http_headers& headers = _response.headers();
            headers["Access-Control-Allow-Origin"] = "*";
            headers["Content-Type"] = "application/json";
        }

        void response::set_body(const json::value& body_data)
        {
            _response.set_body(body_data);
        }

        void response::set_body(const std::string& body_data)
        {
            _response.set_body(body_data);
        }

        void response::send()
        {
            _request.reply(_response);
        }

        /*!
         * \brief Create the server
         */
        server::server()
          : _scheme{"http"},
            _host{"localhost"},
            _port{9080}
        {
        }

        /*!
         * \brief Run the server
         */
        void server::run()
        {
            uri_builder builder;

            builder.set_scheme(_scheme);
            builder.set_host(_host);
            builder.set_port(std::to_string(_port));
            builder.set_path(_path);

            _listener = http_listener{builder.to_uri()};

            _listener.support(
                std::bind(&server::handle_request, this, std::placeholders::_1)
            );

            _listener.support(web::http::methods::OPTIONS, [](http_request request) {
                web::http::http_response response{web::http::status_codes::OK};
                web::http::http_headers& headers = response.headers();
                headers["Access-Control-Allow-Origin"] = "*";
                headers["Access-Control-Allow-Methods"] = "GET, POST, PATCH, PUT, DELETE, OPTIONS";
                headers["Access-Control-Allow-Headers"] = "Origin, Content-Type, X-Auth-Token";
                request.reply(response);
            });

            try
            {
                _listener
                  .open()
                  .then([]() { std::cout << "Listening..." << std::endl; })
                  .wait();

                while (true);
            }
            catch (const std::exception& e)
            {
                std::cout << e.what() << std::endl;
            }
        }

        /*!
         * \brief Run the server on a specific port.
         *
         * \param port the port number
         */
        void server::run(const uint16_t port)
        {
            set_port(port);
            run();
        }

        /*!
         * \brief Register a request handler.
         */
        void server::on(
            const web::http::method& method,
            const std::string& pattern,
            request_handler handler)
        {
            _routes.push_back(request_route{method, std::regex{pattern}, handler});
        }

        void server::handle_request(http_request request)
        {
            auto path = uri::decode(request.relative_uri().path());
            std::smatch match{};

            for (auto& route : _routes) {
                if (request.method() == route.method &&
                    std::regex_match(path, match, route.regex))
                {
                    try 
                    {
                        route.handler(
                            polls::http::request{request, match},
                            polls::http::response{request}
                        );
                        return;
                    } 
                    catch (mongocxx::exception& e) 
                    {
                        std::cout << e.what() << std::endl;
                        std::cout << e.code() << std::endl;
                        polls::http::request req{request};
                        switch (e.code().value())
                        {
                        case 13053:
                            req.send_error_response("No suitable servers found. Is mongod running?");
                            break;
                        case 11000:
                            req.send_error_response("Duplicate key error.", 409);
                            break;
                        default:
                            req.send_error_response(e.what());
                        }
                        return;
                    } 
                    catch (std::exception& e) 
                    {
                        std::cout << e.what() << std::endl;
                        polls::http::request req{request};
                        req.send_error_response(e.what());
                        return;
                    }
                }
            }

            polls::http::response response{request};
            response.set_status_code(status_codes::NotFound);
            response.send();
        }
    }
}
