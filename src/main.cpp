#include <iostream>
#include "ops/http/rest/server.h"
#include "ops/mongodb/pool.h"
#include "ops/controllers/campaigns.h"
#include "ops/controllers/languages.h"
#include "dotenv.h"

int main()
{
    dotenv::init();
    ops::mongodb::pool::init("ops");

    ops::http::rest::server server;

    ops::campaigns_controller campaigns;
    ops::languages_controller languages;

    server.add_controller("campaigns", &campaigns);

    server.add_route(web::http::methods::POST, "^/campaigns/([0-9a-f]+)/features", 
        std::bind(&ops::campaigns_controller::post_feature, &campaigns, std::placeholders::_1));

    server.add_route(web::http::methods::POST, "^/campaigns/([0-9a-f]+)/languages", 
        std::bind(&ops::campaigns_controller::post_language, &campaigns, std::placeholders::_1));

    server.add_controller("languages", &languages);

    server.run();

    return 0;
}
