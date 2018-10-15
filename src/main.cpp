#include <iostream>
#include "src/ops/http/server.h"
#include "src/ops/mongodb/pool.h"

int main()
{
//    ops::mongodb::pool::init("ops");

    ops::http::server server;

//    server.add_controller(ops::campaign_controller);

//    server.run();

    return 0;
}
