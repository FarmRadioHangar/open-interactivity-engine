#include <iostream>
#include "ops/http/rest/server.h"
#include "ops/mongodb/pool.h"
#include "ops/controllers/campaigns.h"
#include "dotenv.h"

int main()
{
    dotenv::init();
    ops::mongodb::pool::init("ops");

    ops::http::rest::server server;

    ops::campaigns_controller campaigns;

    server.add_controller("campaigns", &campaigns);

    server.run();

    return 0;
}
