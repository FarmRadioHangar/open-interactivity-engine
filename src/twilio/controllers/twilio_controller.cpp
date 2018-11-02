#include "twilio_controller.h"
#include <bsoncxx/builder/basic/document.hpp>
#include "../../ops/util/json.h"
#include "../../ops/mongodb/counter.h"
#include "../models/session.h"

namespace twilio
{

using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::basic::make_document;
using web::http::methods;

controller::controller()
  : ops::http::rest::controller{}
{
}

// Called=%2B13163336936&ToState=KS&CallerCountry=UG&Direction=inbound&CallerState=&ToZip=&CallSid=CAd7fe2ad60939a04e564c0e4e3302ff5b&To=%2B13163336936&CallerZip=&ToCountry=US&ApiVersion=2010-04-01&CalledZip=&CalledCity=&CallStatus=ringing&From=%2B256784224203&AccountSid=ACd4a88df477b07e1693a390518517e9f8&CalledCountry=US&CallerCity=&Caller=%2B256784224203&FromCountry=UG&ToCity=&FromCity=&CalledState=KS&FromZip=&FromState=
//
// Called=%2B13163336936
// ToState=KS
// CallerCountry=UG
// Direction=inbound
// CallerState=
// ToZip=
// CallSid=CAd7fe2ad60939a04e564c0e4e3302ff5b
// To=%2B13163336936
// CallerZip=
// ToCountry=US
// ApiVersion=2010-04-01
// CalledZip=
// CalledCity=
// CallStatus=ringing
// From=%2B256784224203
// AccountSid=ACd4a88df477b07e1693a390518517e9f8
// CalledCountry=US
// CallerCity=
// Caller=%2B256784224203
// FromCountry=UG
// ToCity=
// FromCity=
// CalledState=KS
// FromZip=
// FromState=

void controller::post_voice(ops::http::request& request)
{
    request.with_body([this, &request](const std::string& body)
    {
        nlohmann::json j_body = ops::util::json::from_urlencoded(body);

        const std::string session_id = ops::mongodb::counter::generate_id();

        nlohmann::json j_session{
            {"id", session_id},
//            {"campaign", { {"id", campaign_id}} },
            {"conversation", j_body}
        };

        session model(j_session);

        // Register a new session
        ops::mongodb::document<twilio::session>::create(model.builder().extract());

        std::cout << "post_voice: " << std::endl;
        std::cout << body << std::endl;
        std::cout << j_body.dump() << std::endl;

        request.send_response();
    });
}

// Called=%2B13163336936&ToState=KS&CallerCountry=UG&Direction=inbound&Timestamp=Fri%2C%2002%20Nov%202018%2010%3A52%3A29%20%2B0000&CallbackSource=call-progress-events&CallerState=&ToZip=&SequenceNumber=0&To=%2B13163336936&CallSid=CAd7fe2ad60939a04e564c0e4e3302ff5b&ToCountry=US&CallerZip=&CalledZip=&ApiVersion=2010-04-01&CallStatus=completed&CalledCity=&Duration=1&From=%2B256784224203&CallDuration=9&AccountSid=ACd4a88df477b07e1693a390518517e9f8&CalledCountry=US&CallerCity=&ToCity=&FromCountry=UG&Caller=%2B256784224203&FromCity=&CalledState=KS&FromZip=&FromState=
//
// Called=%2B13163336936
// ToState=KS
// CallerCountry=UG
// Direction=inbound
// Timestamp=Fri%2C%2002%20Nov%202018%2010%3A52%3A29%20%2B0000
// CallbackSource=call-progress-events
// CallerState=
// ToZip=
// SequenceNumber=0
// To=%2B13163336936
// CallSid=CAd7fe2ad60939a04e564c0e4e3302ff5b
// ToCountry=US
// CallerZip=
// CalledZip=
// ApiVersion=2010-04-01
// CallStatus=completed
// CalledCity=
// Duration=1
// From=%2B256784224203
// CallDuration=9
// AccountSid=ACd4a88df477b07e1693a390518517e9f8
// CalledCountry=US
// CallerCity=
// ToCity=
// FromCountry=UG
// Caller=%2B256784224203
// FromCity=
// CalledState=KS
// FromZip=
// FromState=

void controller::post_event(ops::http::request& request)
{
    request.with_body([this, &request](const std::string& body)
    {
        std::cout << "post_event: " << std::endl;
        std::cout << body << std::endl;

        request.send_response();
    });
}

void controller::do_install(ops::http::rest::server* server)
{
    server->on(methods::POST, "^/twilio/voice$",
        bind_handler<controller>(&controller::post_voice));

    server->on(methods::POST, "^/twilio/event$",
        bind_handler<controller>(&controller::post_event));

//    server->on(methods::POST, "^/twilio/ivr/s/([0-9a-f]+)/n/([0-9]+)$",
//        bind_handler<controller>(&controller::post_ivr));
//
//    server->on(methods::POST, "^/twilio/event$",
//        bind_handler<controller>(&controller::post_event));
//
//    server->on(methods::POST, "^/twilio/answer/c/([0-9a-f]+)/f/([0-9a-f]+)$",
//        bind_handler<controller>(&controller::post_answer));
}

} // namespace twilio
