#include "nexmo_voice.h"
#include <cassert>

namespace nexmo
{
  
voice::voice()
  : ops::adapter{}
{
}

void voice::do_install(ops::http::rest::server* server)
{
    assert(server);
}

} // namespace nexmo
