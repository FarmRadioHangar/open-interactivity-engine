/*!
 * \file controller.h
 */
#pragma once

#include "server.h"

namespace ops
{
    enum route_type 
    {
        t_get,
        t_get_one,
        t_post,
        t_put,
        t_delete
    };

    template <typename T> class rest_controller
    {
    public:
        rest_controller(http::server* const server);

        virtual ~rest_controller() = default;

        rest_controller(const rest_controller&) = delete;
        rest_controller& operator=(const rest_controller&) = delete;

        void register_route(const route_type type, const std::string& pattern);

        void get_one(ops::http::request request);
        void get(ops::http::request request);
        void post(ops::http::request request);
        void put(ops::http::request request);
        void del(ops::http::request request);

    private:
        http::server* const _server;
    };

    /*!
     * \brief todo
     */
    template <typename T> 
    rest_controller<T>::rest_controller(http::server* const server)
      : _server{server}
    {
    }

    /*!
     * \brief todo
     */
    template <typename T> 
    void rest_controller<T>::register_route(const route_type type, const std::string& pattern)
    {
        switch (type)
        {
        case t_get:
            _server->on(web::http::methods::GET, pattern, 
                std::bind(&rest_controller<T>::get, this, std::placeholders::_1));
            break;
        case t_get_one:
            _server->on(web::http::methods::GET, pattern, 
                std::bind(&rest_controller<T>::get_one, this, std::placeholders::_1));
            break;
        case t_post:
            _server->on(web::http::methods::POST, pattern, 
                std::bind(&rest_controller<T>::post, this, std::placeholders::_1));
            break;
        case t_put:
            _server->on(web::http::methods::PUT, pattern, 
                std::bind(&rest_controller<T>::put, this, std::placeholders::_1));
            break;
        case t_delete:
            _server->on(web::http::methods::DEL, pattern, 
                std::bind(&rest_controller<T>::del, this, std::placeholders::_1));
            break;
        }
    }

    /*!
     * \brief todo
     */
    template <typename T> 
    void rest_controller<T>::get_one(ops::http::request request)
    {
        auto document = T::get(request.get_uri_param(1));
        web::json::value response{};
        response[T::mongodb_collection] = document.to_json();

        request.send_response(response);
    }

    /*!
     * \brief todo
     */
    template <typename T> 
    void rest_controller<T>::get(ops::http::request request)
    {
        auto skip = request.get_query_param<int64_t>("skip", 0);
        auto limit = request.get_query_param<int64_t>("limit", 10);

        request.send_response(T::page(skip, limit).to_json());
    }

    /*!
     * \brief todo
     */
    template <typename T> 
    void rest_controller<T>::post(ops::http::request request)
    {
        request.with_body([&request](const std::string& body)
        {
            T document{};
            
            document.set_data(body);
            document.validate();
            document.save();

            web::json::value response{};
            response[T::mongodb_collection] = document.to_json();

            request.send_response(response);
        });
    }

    /*!
     * \brief todo
     */
    template <typename T> 
    void rest_controller<T>::put(ops::http::request request)
    {
        request.with_body([&request](const std::string& body)
        {
            auto document = T::get(request.get_uri_param(1));
 
            document.set_data(body);
            document.validate();
            document.save();

            web::json::value response{};
            response[T::mongodb_collection] = document.to_json();

            request.send_response(response);
        });
    }

    /*!
     * \brief todo
     */
    template <typename T> 
    void rest_controller<T>::del(ops::http::request request)
    {
        auto document = T::get(request.get_uri_param(1));
        document.remove();

        request.set_status_code(web::http::status_codes::NoContent);
        request.send_response();
    }
}
