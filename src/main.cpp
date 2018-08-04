#include <iostream>
#include <mongocxx/instance.hpp>
#include <thread>
#include "model.h"
#include "server.h"
#include "trace.h"

class pants : public polls::model<pants>
{
public:
    //static constexpr const char* collection_name = "pants";

    pants() : polls::model<pants>{"test", "pants"} {}
};

class campaign : public polls::model<campaign>
{
    static constexpr auto mongodb_collection = "campaigns";

public:
    campaign() : polls::model<campaign>{"uliza", campaign::mongodb_collection} 
    {}
};

int main()
{
    mongocxx::instance instance{};

    //A<s>::cout();

    campaign doc{};
    doc = campaign::get("xxxxxx86be9ca51a9a3c0e81");

    //pants pant{};

    //campaign doc{};

//    std::cout << "xxx" << std::endl;
//
////    std::cout << pants::db_name << std::endl;
//
//
//    campaign document = campaign::get("5b63f486be9ca51a9a3c0e81");
//
//    //std::cout << doucument.data() << std::endl;
//
//    //document.set_oid("5b63f486be9ca51a9a3c0e81");
//
//    document.fetch();
//    document.save();

//
//    //campaign document = campaign::get("5b63f486be9ca51a9a3c0e81");
//
//    document.set_data("{\"baz\"\"fez\"}");
//
//    document.save();

//    try {
//        document.fetch();
//    } catch(std::exception& e) {
//        std::cout << e.what() << std::endl;
//    }
//
//    document.set_oid("5b4a1f7bd93a9362f2cae08b");
//
//    try {
//        document.fetch();
//        std::cout << document.oid().to_string() << std::endl;
//    } catch(std::exception& e) {
//        std::cout << e.what() << std::endl;
//    }
//
//    document.remove();
//
    //c = campaign::get("5b4a1f7bd93a9362f2cae08b");

    //std::cout << c.id().to_string() << std::endl;

    //std::vector<std::thread> threads;

    //for (int i = 0; i < 800; i++)
    //{
    //    std::thread t{[]() {
    //        campaign campaign_obj = campaign::get("5b4a1f7bd93a9362f2cae08b");
    //    }};
    //    threads.push_back(std::move(t));
    //}

    //std::vector<std::thread>::iterator i;
    //for (i = threads.begin(); i != threads.end(); ++i)
    //{
    //    i->join();
    //}

    return 0;
}
