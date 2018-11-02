#include "json.h"
#include <boost/algorithm/string.hpp>

namespace ops
{
namespace util
{
namespace json
{

void urldecode(char *dst, const char *src)
{
    char a, b;

    while (*src)
    {
        if ((*src == '%') && ((a = src[1]) && (b = src[2])) && (isxdigit(a) && isxdigit(b)))
        {
            if (a >= 'a') {
                a -= ('a' - 'A');
            }
            if (a >= 'A') {
                a -= ('A' - 10);
            } else {
                a -= '0';
            }
            if (b >= 'a') {
                b -= ('a' - 'A');
            }
            if (b >= 'A') {
                b -= ('A' - 10);
            } else {
                b -= '0';
            }
            *dst++ = 16*a + b;
            src += 3;
        } else if (*src == '+') {
            *dst++ = ' ';
            src++;
        } else {
            *dst++ = *src++;
        }
    }

    *dst++ = '\0';
}

nlohmann::json from_urlencoded(const std::string& input)
{
    char value[4096];

    std::vector<std::string> pairs;
    boost::split(pairs, input, [](char c){ return c == '&'; });

    auto obj = nlohmann::json::object();

    for (const auto& kvp : pairs)
    {
        const auto equals_idx = kvp.find_first_of('=');
        if (std::string::npos != equals_idx)
        {
            urldecode(value, kvp.substr(equals_idx + 1).c_str());
            obj[kvp.substr(0, equals_idx)] = value;
        }
    }

    return obj;
}

}
}
}
