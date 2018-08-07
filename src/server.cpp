#include "server.h"

#include <cpprest/http_listener.h>

using namespace web;
using namespace http::experimental::listener;

namespace polls
{
    namespace http
    {
        /*!
         * \brief Create the server
         */
        server::server()
          : _port{9080}
        {
        }

        /*!
         * \brief Run the server
         */
        void server::run()
        {
        }

        /*!
         * \brief Run the server on a specific port.
         */
        void server::run(const uint16_t port)
        {
            set_port(port);
            run();
        }
    }
}
