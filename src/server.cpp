#include "server.h"
#include <mongocxx/exception/exception.hpp>
#include <cpprest/uri_builder.h>
#include <cpprest/http_headers.h>
#include "model/exception.h"

namespace survey
{
    namespace http
    {
        request::request(web::http::http_request&& request, const boost::smatch& match)
          : _match{match},
            _params{web::uri::split_query(request.request_uri().query())},
            _request{request},
            _response{web::http::status_codes::OK}
        {
            web::http::http_headers& headers = _response.headers();
            headers["Access-Control-Allow-Origin"] = "*";
            headers["Content-Type"] = "application/json";
        }

        void request::with_json(std::function<void(web::json::value)> handler)
        {
            _request.extract_json().then(handler).wait();
        }

        void request::with_body(std::function<void(const std::string&)> handler)
        {
            _request.extract_string().then(handler).wait();
        }

        void request::send_response()
        {
            _request.reply(_response);
        }

        void request::send_response(const web::json::value& json)
        {
            _response.set_body(json);
            _request.reply(_response);
        }

        void request::send_response(const std::string& data)
        {
            _response.set_body(data);
            _request.reply(_response);
        }

        void request::send_error_response(
            web::http::status_code code,
            const std::string& atom,
            const std::string& error)
        {
            web::json::value response{};
            response["status"] = web::json::value::number(code);
            response["error"]  = web::json::value::string(error);
            response["code"] = web::json::value::string(atom);
            set_status_code(code);
            send_response(response);
        }

        /*!
         * \brief Create the server.
         */
        server::server(const uint16_t port,
                       const std::string& scheme,
                       const std::string& host,
                       const std::string& path)
          : _port{port},
            _scheme{scheme},
            _host{host},
            _path{path}
        {
        }

        /*!
         * \brief Run the server.
         */
        void server::run()
        {
            using web::http::experimental::listener::http_listener;
            using namespace web::http;

            uri_builder builder{};

            builder.set_scheme(_scheme);
            builder.set_host(_host);
            builder.set_path(_path);
            builder.set_port(std::to_string(_port));

            _listener = http_listener{builder.to_uri()};

            _listener.support(
                std::bind(&server::handle_request, this, std::placeholders::_1)
            );

            _listener.support(methods::OPTIONS, [](http_request request) {
                http_response response{status_codes::OK};
                http_headers& headers = response.headers();
                headers["Access-Control-Allow-Origin"] = "*";
                headers["Access-Control-Allow-Methods"] = "GET, POST, PATCH, PUT, DELETE, OPTIONS";
                headers["Access-Control-Allow-Headers"] = "Origin, Content-Type, X-Auth-Token";
                request.reply(response);
            });

            try {
              _listener.open()
                       .then([this]() { std::cout << "Listening on port " << _port << "..." << std::endl; })
                       .wait();
              while (true);
            } catch (const std::exception& e) {
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
         *
         * \param method      todo
         * \param uri_pattern todo
         * \param handler     todo
         */
        void server::on(const web::http::method& method,
                        const std::string& uri_pattern,
                        request_handler handler)
        {
            _routes.emplace_back(
                request_route{method, boost::regex{uri_pattern}, handler}
            );
        }

        void server::handle_request(web::http::http_request request)
        {
            auto path = web::http::uri::decode(request.relative_uri().path());
            boost::smatch match{};

            for (const auto& route : _routes) {
                if (request.method() == route.method
                    && boost::regex_search(path, match, route.regex))
                {
                    http::request req{std::move(request), match};

                    try {
                        route.handler(req);
                        return;
                    } catch (const web::json::json_exception& error) {
                        req.send_error_response(400, "BAD_JSON", error.what());
                        return;
                    } catch (const mongocxx::exception& error) {
                        switch (error.code().value())
                        {
                        case 13053:
                            req.send_error_response(502, "BAD_GATEWAY", 
                                "No suitable servers found. Is mongod running?");
                            break;
                        case 11000:
                            req.send_error_response(409, "DUPLICATE_KEY", 
                                "Duplicate key error.");
                            break;
                        default:
                            req.send_error_response(500, "INTERNAL_SERVER_ERROR",
                                error.what());
                        }
                        return;
//                    } catch (const survey::model_error& error) {
//                        //
                    } catch (const std::exception& error) {
                        req.send_error_response(500, "INTERNAL_SERVER_ERROR", error.what());
                        return;
                    }
                }
            }

            // send 404 response
            http::request req{std::move(request), match};
            req.send_error_response(404, "NOT_FOUND", "Not found");
        }
    }
}
