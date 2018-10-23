#include <iostream>
#include "ops/http/rest/server.h"
#include "ops/mongodb/pool.h"
#include "ops/controllers/campaigns.h"
#include "ops/controllers/content.h"
#include "ops/controllers/languages.h"
#include "dotenv.h"

int main()
{
    using web::http::methods;

    dotenv::init();
    ops::mongodb::pool::init("ops");

    ops::http::rest::server server;

    auto campaigns = std::make_unique<ops::campaigns_controller>();
    auto languages = std::make_unique<ops::languages_controller>();
    auto content = std::make_unique<ops::content_controller>();

    server.add_controller("campaigns", campaigns.get());

    server.add_route(methods::POST, "^/campaigns/([0-9a-f]+)/features$",
        campaigns->bind_handler<ops::campaigns_controller>(&ops::campaigns_controller::post_feature));

    server.add_route(methods::PATCH, "^/campaigns/([0-9a-f]+)/features/([0-9a-f]+)$",
        campaigns->bind_handler<ops::campaigns_controller>(&ops::campaigns_controller::patch_feature));

    server.add_route(methods::POST, "^/campaigns/([0-9a-f]+)/features/([0-9a-f]+)/adapters$",
        campaigns->bind_handler<ops::campaigns_controller>(&ops::campaigns_controller::post_adapter));

    server.add_route(methods::POST, "^/campaigns/([0-9a-f]+)/languages$",
        campaigns->bind_handler<ops::campaigns_controller>(&ops::campaigns_controller::post_language));

    server.add_controller("languages", languages.get());
    server.add_controller("content", content.get());

    server.add_route(methods::POST, "^/content/([0-9a-f]+)/reps$",
        content->bind_handler<ops::content_controller>(&ops::content_controller::post_rep));

    server.add_route(methods::POST, "^/media$",
        content->bind_handler<ops::content_controller>(&ops::content_controller::post_media));

    server.run();

    return 0;
}
