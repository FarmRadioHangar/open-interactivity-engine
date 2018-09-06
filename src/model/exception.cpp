#include "exception.h"

namespace survey
{
    model_error::model_error(const model_error_type type, const std::string& message)
      : std::runtime_error{message},
        _type{type}
    {
    }
}
