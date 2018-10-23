///
/// \file adapter.h
///
#pragma once

namespace ops
{
    namespace http
    {
        namespace rest
        {
            class server;
        }
    }

    class adapter
    {
    public:
        virtual ~adapter() = default;

        adapter();

        adapter(const adapter&) = delete;
        adapter& operator=(const adapter&) = delete;

        void install(ops::http::rest::server* server);

    private:
        virtual void do_install(ops::http::rest::server* server) = 0;
    };

    inline void adapter::install(ops::http::rest::server* server)
    {
        do_install(server);
    }
}
