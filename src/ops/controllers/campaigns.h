///
/// \file campaigns.h
///
#pragma once

#include "../http/rest/controller.h"

namespace ops
{
    class campaigns_controller : public http::rest::controller
    {
    public:
        campaigns_controller();

        void get_item(http::request request) override;
        void get(http::request request) override;
        void post(http::request request) override;

    private:
        void post_feature(http::request request);
        void post_language(http::request request);
    };
}
