/*!
 * \file server.h
 */
#pragma once

namespace polls
{
    namespace http
    {
        class server
        {
        public:
            server();
            ~server() = default;
            server(const server&) = delete;
            server& operator=(const server&) = delete;

            void run();
            void run(const uint16_t port);

            /*!
             * \brief Configure the server to listen on the given port for
             * incoming connections.
             */
            void set_port(const uint16_t port) { _port = port; };

        private:
            uint16_t _port;
        };
    }
}
