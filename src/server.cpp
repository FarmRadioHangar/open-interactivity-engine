#include "server.h"

using namespace web;
using namespace web::http;
using namespace http::experimental::listener;

namespace polls
{
    namespace http
    {
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
                  .then([]() { std::cout << "listening..." << std::endl; })
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

            web::http::http_response response{web::http::status_codes::OK};
            web::http::http_headers& headers = response.headers();
            headers["Access-Control-Allow-Origin"] = "*";
            headers["Content-Type"] = "application/json";

            for (auto& route : _routes) {
                if (request.method() == route.method &&
                    std::regex_match(path, match, route.regex))
                {
                    try {
                        route.handler(request, response, match);
                        return;
                    } catch(std::exception& e) {
                        std::cout << e.what() << std::endl;
                        std::stringstream oss{};
                        oss << "{\"error\":\"" << e.what() << "\"}";
                        response.set_status_code(status_codes::InternalError);
                        response.set_body(oss.str());
                        request.reply(response);
                        return;
                    }
                }
            }

            response.set_status_code(status_codes::NotFound);
            request.reply(response);
        }
    }
}
