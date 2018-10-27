#include <iostream>
#include "ops/http/rest/server.h"
#include "ops/mongodb/pool.h"
#include "core/controllers/campaigns.h"
#include "core/controllers/content.h"
#include "core/controllers/languages.h"
#include "core/controllers/media.h"
#include "dotenv/dotenv.h"
#include "nexmo/adapters/nexmo_voice.h"

int main()
{
    dotenv::init();
    ops::mongodb::pool::init("ops");

    ops::http::rest::server server;

    //

    auto campaigns = std::make_unique<ops::campaigns_controller>();
    auto languages = std::make_unique<ops::languages_controller>();
    auto content = std::make_unique<ops::content_controller>();
    auto media = std::make_unique<ops::media_controller>();
//    auto audience = std::make_unique<ops::audience_controller>();
//    auto country = std::make_unique<ops::country_controller>();

    server.add_controller("campaigns", campaigns.get());
    server.add_controller("languages", languages.get());
    server.add_controller("content", content.get());
    server.add_controller("media", media.get());
//    server.add_controller("audience", audience.get());
//    server.add_controller("country", country.get());

    //

    auto nexmo_voice = std::make_unique<nexmo::voice>();

    server.register_adapter(nexmo_voice.get());

    //

    server.run();

    return 0;
}
