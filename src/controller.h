/*!
 * \file controller.h
 */
#pragma once

#include "server.h"

namespace ops
{
    namespace rest
    {
        enum route_type
        {
            t_get,
            t_get_one,
            t_post,
            t_put,
            t_delete
        };

        template <typename T> class controller
        {
        public:
            explicit controller(http::server* const server);

            virtual ~controller() = default;

            controller(const controller&) = delete;
            controller& operator=(const controller&) = delete;

            void register_route(const rest::route_type type, const std::string& pattern);
            void register_default_routes(const std::string& id_pattern = "[0-9a-f]+");

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
        controller<T>::controller(http::server* const server)
          : _server{server}
        {
        }

        /*!
         * \brief todo
         */
        template <typename T>
        void controller<T>::register_route(const rest::route_type type, const std::string& pattern)
        {
            using namespace rest;

            switch (type)
            {
            case t_get:
                _server->on(web::http::methods::GET, pattern,
                    std::bind(&controller<T>::get, this, std::placeholders::_1));
                break;
            case t_get_one:
                _server->on(web::http::methods::GET, pattern,
                    std::bind(&controller<T>::get_one, this, std::placeholders::_1));
                break;
            case t_post:
                _server->on(web::http::methods::POST, pattern,
                    std::bind(&controller<T>::post, this, std::placeholders::_1));
                break;
            case t_put:
                _server->on(web::http::methods::PUT, pattern,
                    std::bind(&controller<T>::put, this, std::placeholders::_1));
                break;
            case t_delete:
                _server->on(web::http::methods::DEL, pattern,
                    std::bind(&controller<T>::del, this, std::placeholders::_1));
                break;
            }
        }

        /*!
         * \brief todo
         */
        template <typename T>
        void controller<T>::register_default_routes(const std::string& id_pattern)
        {
            using namespace rest;

            const std::string base_ex{"^/" + std::string{T::mongodb_collection} + "$"};
            const std::string item_ex{"^/" + std::string{T::mongodb_collection} + "/(" + id_pattern + ")$"};

            register_route(t_get, base_ex);
            register_route(t_post, base_ex);
            register_route(t_get_one, item_ex);
            register_route(t_put, item_ex);
            register_route(t_delete, item_ex);
        }

        /*!
         * \brief todo
         *
         * \param request todo
         */
        template <typename T>
        void controller<T>::get_one(ops::http::request request)
        {
            auto document = T::get(request.get_uri_param(1));
            web::json::value response{};
            response[T::mongodb_collection] = document.to_json();

            request.send_response(response);
        }

        /*!
         * \brief todo
         *
         * \param request todo
         */
        template <typename T>
        void controller<T>::get(ops::http::request request)
        {
            auto skip = request.get_query_param<int64_t>("skip", 0);
            auto limit = request.get_query_param<int64_t>("limit", 10);

            request.send_response(T::page(skip, limit).to_json());
        }

        /*!
         * \brief todo
         *
         * \param request todo
         */
        template <typename T>
        void controller<T>::post(ops::http::request request)
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
         *
         * \param request todo
         */
        template <typename T>
        void controller<T>::put(ops::http::request request)
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
         *
         * \param request todo
         */
        template <typename T>
        void controller<T>::del(ops::http::request request)
        {
            auto document = T::get(request.get_uri_param(1));
            document.remove();

            request.set_status_code(web::http::status_codes::NoContent);
            request.send_response();
        }
    }
}
