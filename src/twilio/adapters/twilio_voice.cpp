#include "twilio_voice.h"

namespace twilio
{

voice::voice()
  : ops::adapter{},
    _ctrl{std::make_unique<controller>()}
{
}

void voice::do_install(ops::http::rest::server* server)
{
    assert(server);

    _ctrl->install(server);
}

} // namespace twilio
