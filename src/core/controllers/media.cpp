#include "media.h"
#include <fstream>
#include <nlohmann/json.hpp>
#include "../../ops/mongodb/counter.h"
#include "../../ops/mongodb/document.h"
#include "../../ops/mongodb/page.h"
#include "../../ops/util/json.h"
#include "../models/media.h"

namespace core
{

using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::basic::make_document;

media_controller::media_controller()
  : ops::http::rest::controller{}
{
}

void media_controller::get_item(ops::http::request& request)
{
    const auto media_id = request.get_uri_param(1);
    const std::string file = std::string{media_id} + ".mp3";

    request.send_media_response(file, "audio/mpeg");
}

void media_controller::post(ops::http::request& request)
{
    request.with_body([&request](const std::vector<unsigned char>& bytes)
    {
        nlohmann::json j_media;
        j_media["id"] = ops::mongodb::counter::generate_id();
        j_media["file"] = std::string{j_media["id"]} + ".mp3";

        std::ofstream outfile(j_media["file"], std::ios::out | std::ios::binary); 
        outfile.write(reinterpret_cast<const char*>(bytes.data()), bytes.size());

        media model(j_media);

        ops::mongodb::document<media>::create(model.builder().extract());

        request.send_response({ {"media", j_media} });
    });
}

} // namespace core
