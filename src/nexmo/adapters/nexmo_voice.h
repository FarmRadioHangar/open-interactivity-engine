///
/// \file nexmo_voice.h
///
#pragma once

#include "../../ops/adapter.h"
#include "../controllers/nexmo_controller.h"

namespace nexmo
{
    class voice : public ops::adapter
    {
    public:
        voice();

    private:
        void do_install(ops::http::rest::server* server) override;

        std::unique_ptr<controller> _ctrl;
    };
}
