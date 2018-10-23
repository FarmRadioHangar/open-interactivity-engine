///
/// \file nexmo_voice.h
///
#pragma once

#include "../ops/adapter.h"

namespace nexmo
{
    class voice : public ops::adapter
    {
    public:
        voice();

    private:
        void do_install(ops::http::rest::server* server) override;
    };
}
