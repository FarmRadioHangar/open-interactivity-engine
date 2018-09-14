//#include "exception.h"
//
//namespace polls
//{
//    namespace utils
//    {
//        namespace builder
//        {
//            error::error(
//                const error_type   type,
//                const std::string& message,
//                const int          status)
//              : std::runtime_error{message},
//                _type{type},
//                _status{status}
//            {
//            }
//
//            std::string error::atom() const
//            {
//                switch (_type)
//                {
//                case json_not_an_object:
//                    return "JSON_NOT_AN_OBJECT";
//                case missing_property:
//                    return "MISSING_PROPERTY";
//                case type_mismatch:
//                    return "TYPE_MISMATCH";
//                case unique_constraint_violation:
//                    return "UNIQUE_CONSTRAINT_VIOLATION";
//                default:
//                    return "BAD_REQUEST";
//                }
//            }
//
//            web::json::value error::to_json_impl() const
//            {
//                web::json::value json_response{};
//                json_response["error"] = web::json::value::string(what());
//                json_response["status"] = web::json::value::number(_status);
//                json_response["code"] = web::json::value::string(atom());
//                return json_response;
//            }
//
//            key_validation_error::key_validation_error(
//                const std::string& key,
//                const error_type   type,
//                const std::string& message,
//                const int          status)
//              : error{type, message, status},
//                _key{key}
//            {
//            }
//
//            web::json::value key_validation_error::to_json_impl() const
//            {
//                web::json::value json_response = error::to_json_impl();
//                json_response["key"] = web::json::value::string(_key);
//                return json_response;
//            }
//        }
//    }
//}
