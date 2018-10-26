#include "server.h"
#include <cpprest/filestream.h>

namespace ops
{
namespace http
{

///
/// \class request
///
/// \brief HTTP request container
///

///
/// \brief Create a request object.
///
/// \param request original REST SDK request object
/// \param match   regex results when matching against URL
///
request::request(web::http::http_request&& request, const boost::smatch& match)
  : _match{match},
    _params{web::uri::split_query(request.request_uri().query())},
    _request{std::move(request)},
    _response{web::http::status_codes::OK}
{
    web::http::http_headers& headers = _response.headers();
    headers["Access-Control-Allow-Origin"] = "*";
    headers["Content-Type"] = "application/json";
}

///
/// \brief Extract the request body and pass it to the provided callback.
///
/// \param handler callback which will receive the request body
///
void request::with_body(std::function<void(const std::string&)> handler)
{
    std::cout << "request::with_body [in]" << std::endl;
    _request.extract_string().then(handler).wait();
    std::cout << "request::with_body [out]" << std::endl;
}

///
/// \brief Extract the request body and pass it to the provided callback as a 
///        byte vector.
///
/// \param handler callback which will receive the request body
///
void request::with_body(std::function<void(const std::vector<unsigned char>&)> handler)
{
    _request.extract_vector().then(handler).wait();
}

///
/// \brief Send a response with a 200 OK status code.
///
/// \param body todo
///
void request::send_response(const std::string& body)
{
    _response.set_body(body);
    _request.reply(_response);
}

///
/// \brief todo
///
/// \param body todo
///
void request::send_response(const nlohmann::json& j)
{
    send_response(j.dump());
}

///
/// \brief Send a reponse to indicate that an error occurred.
///
/// \param code  todo
/// \param tag   todo
/// \param error todo
///
void request::send_error_response(web::http::status_code code,
                                  const std::string& tag,
                                  const std::string& error)
{
    nlohmann::json response{};
    response["status"] = code;
    response["error"] = error;
    response["code"] = tag;
    set_status_code(code);
    send_response(response.dump());
}

void request::send_media_response(const std::string& file, const std::string& format)
{
    using namespace Concurrency::streams;

    file_stream<unsigned char>::open_istream(file)
        .then([this, format](const basic_istream<unsigned char>& istream) {
            _request.reply(web::http::status_codes::OK, istream, format);
        })
        .wait();
}

///
/// \struct request::route
///
/// \brief Route handler
///

///
/// \fn request::get_uri_param
///
/// \brief Obtain the *n*-th URI parameter from the list of matches.
///
/// \param n todo
///
/// \returns todo
///

///
/// \fn request::get_query_param
///
/// \brief todo
///
/// \param name todo
/// \param def  todo
///
/// \returns todo
///

///
/// \fn request::set_status_code
///
/// \brief todo
///

///
/// \class server
///
/// \brief REST server
///

///
/// \brief Create the server.
///
/// \param port   number of the port to listen on for new connections (9080)
/// \param scheme the scheme identifying the protocol used (http)
/// \param host   todo
/// \param path   todo
///
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

///
/// \brief Run the server.
///
void server::run()
{
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

///
/// \brief Run the server on a specific port.
///
/// \param port the port number to listen on
///
void server::run(const uint16_t port)
{
    set_port(port);
    run();
}

///
/// \brief Register a request handler.
///
/// \param method      HTTP method to respond to
/// \param uri_pattern a regular expression that the request URI must match
/// \param handler     a callback which will be used to handle the request
///
void server::on(web::http::method method,
                const std::string& uri_pattern,
                request::handler handler)
{
    request::route route{method, boost::regex{uri_pattern}, handler};
    _routes.emplace_back(route);
}

void server::handle_request(web::http::http_request request)
{
    auto path = web::http::uri::decode(request.relative_uri().path());
    boost::smatch match{};

    for (const auto& route : _routes) {
        if (request.method() == route.method
            && boost::regex_search(path, match, route.pattern))
        {
            http::request req{std::move(request), match};

            try {
                std::cout << "route.handler [in]" << std::endl;
                route.handler(req);
                std::cout << "route.handler [out]" << std::endl;
                return;
            //} catch (const web::json::json_exception& error) {
            //    req.send_error_response(400, "BAD_JSON", error.what());
            //    return;
            //} catch (const mongocxx::exception& error) {
            //    switch (error.code().value())
            //    {
            //    case 13053:
            //        req.send_error_response(502, "BAD_GATEWAY",
            //            "No suitable servers found. Is mongod running?");
            //        break;
            //    case 11000:
            //        req.send_error_response(409, "DUPLICATE_KEY",
            //            "Duplicate key error.");
            //        break;
            //    default:
            //        req.send_error_response(500, "INTERNAL_SERVER_ERROR",
            //            error.what());
            //    }
            //    return;
            //} catch (const ops::model_error& error) {
            //    switch (error.type())
            //    {
            //    case model_error::validation_error:
            //        req.send_error_response(400, "VALIDATION_FAILED", error.json_data());
            //        break;
            //    case model_error::document_not_found:
            //        req.send_error_response(404, "NOT_FOUND", "No such document");
            //        break;
            //    case model_error::bad_oid:
            //        req.send_error_response(400, "BAD_OID", "Bad ObjectId");
            //        break;
            //    case model_error::bad_bson_data:
            //    default:
            //        req.send_error_response(400, "BAD_BSON", "Not a valid document");
            //    }
            //    return;
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

///
/// \fn server::set_port
///
/// \brief Configure the server to listen on the given port for incoming
///        connections.
///
/// \param port the port number
///

} // namespace http
} // namespace ops
