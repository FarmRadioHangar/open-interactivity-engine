#include "error.h"

namespace ops
{
    validation_error::validation_error(const std::string& key, const std::string& message)
      : _key{key},
        _message{message}
    {
    }

    web::json::value validation_error::to_json() const
    {
        web::json::value obj{};
        obj["key"] = web::json::value::string(_key);
        obj["message"] = web::json::value::string(_message);
        return obj;
    }
}

