#include "exception.h"

namespace polls
{
    namespace utils
    {
        namespace builder
        {
            error::error(error_type type, const std::string& message)
              : std::runtime_error{message},
                _type{type}
            {
            }

            key_validation_error::key_validation_error(error_type type, const std::string& message, const std::string& key)
              : error{type, message},
                _key{key}
            {
            }
        }
    }
}
