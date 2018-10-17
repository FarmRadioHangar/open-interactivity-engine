#include "content.h"
#include <nlohmann/json.hpp>
#include "../mongodb/counter.h"
#include "../mongodb/document.h"
#include "../mongodb/page.h"
#include "../builders/content.h"

struct content
{
    static auto constexpr name = "content";
};

namespace ops
{
    namespace util
    {
        namespace json
        {
            template <typename T>
            nlohmann::json builder(const mongodb::document<T>& doc)
            {
                nlohmann::json j;
                doc.stream() >> j;

                j.erase("_id");

                return j;
            }
        }
    }
}

namespace ops
{

using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::basic::make_document;

content_controller::content_controller()
  : http::rest::controller{}
{
}

void content_controller::get_item(http::request request)
{
    const auto id = request.get_uri_param(1);
    const auto doc = mongodb::document<content>::find(make_document(kvp("id", id)));

    nlohmann::json res;
    res["content"] = util::json::builder(doc);

    request.send_response(res.dump());
}

void content_controller::get(http::request request)
{
    const auto skip = request.get_query_param<int64_t>("skip", 0);
    const auto limit = request.get_query_param<int64_t>("limit", 10);

    auto page = ops::mongodb::page<content>::get(skip, limit);

    auto items = nlohmann::json::array();
    for (const auto& doc : page)
        items.emplace_back(util::json::builder(doc));

    nlohmann::json res;
    res["content"] = items;

    request.send_response(res.dump());
}

void content_controller::post(http::request request)
{
    request.with_body([&request](const std::string& body)
    {
        auto j = nlohmann::json::parse(body);
        j["id"] = ops::mongodb::counter::generate_id();

        content_builder builder(j);

        mongodb::document<content> doc{};
        doc.inject(builder.extract());
        doc.save();

        nlohmann::json res;
        res["content"] = j;

        request.send_response(res.dump());
    });
}

} // namespace ops
