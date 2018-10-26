#include "media.h"
#include <fstream>
#include <nlohmann/json.hpp>
#include "../mongodb/counter.h"
#include "../mongodb/document.h"
#include "../mongodb/page.h"
#include "../builders/language.h"
#include "../util/json.h"

namespace ops
{

using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::basic::make_document;

media_controller::media_controller()
  : http::rest::controller{}
{
}

void media_controller::get_item(http::request request)
{
    request.send_media_response("407af98c43fd.mp3", "audio/mpeg");
}

} // namespace ops
