#include <iostream>
#include "ops/http/server.h"
#include "ops/mongodb/pool.h"

int main()
{
    ops::mongodb::pool::init("ops");

    ops::http::server server;

//    server.add_controller(ops::campaign_controller);

//    server.run();

    return 0;
}
