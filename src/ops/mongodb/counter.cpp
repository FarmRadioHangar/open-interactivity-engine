#include "counter.h"
#include <bsoncxx/builder/basic/document.hpp>
#include <bsoncxx/builder/basic/kvp.hpp>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <sstream>

namespace ops
{
namespace mongodb
{

std::uint64_t counter::next()
{
    using bsoncxx::builder::basic::kvp;
    using bsoncxx::builder::basic::sub_document;

    std::lock_guard<std::mutex> guard(_mutex);

    static std::uint64_t counter = 0;
    static std::uint64_t available = 0;

    if (0 == available--)
    {
        const auto db = ops::mongodb::pool::instance().database();
        auto collection = db.collection("counter");

        bsoncxx::builder::basic::document builder{};
        builder.append(kvp("$inc", [](sub_document subdoc) {
            subdoc.append(kvp("COUNT", Increment));
        }));

        mongocxx::options::find_one_and_replace options{};
        options.upsert(true);

        auto result = collection.find_one_and_replace({}, builder.view(), options);

        if (result) {
            const auto view = result.value().view();
            const auto& v_count = view["COUNT"];
            if (bsoncxx::type::k_int64 == v_count.type()) {
                counter = v_count.get_int64().value;
            } else if (bsoncxx::type::k_int32 == v_count.type()) {
                counter = v_count.get_int32().value;
            } else if (bsoncxx::type::k_double == v_count.type()) {
                counter = v_count.get_double().value;
            } else {
                throw std::runtime_error{"unexpected counter type"};
            }
        } else {
            counter = 0;
        }

        available = Increment - 1;
    }

    ++counter;

    return counter * 83211077003543;
}

std::string counter::generate_id()
{
    const std::uint64_t id = next();

    std::stringstream id_stream;
    id_stream << std::hex << std::setw(12) << std::setfill('0') << id;

    std::string str = id_stream.str();
    const int8_t diff = str.size() - 12;

    return diff > 0 ? str.substr(diff, 12) : str;
}

std::mutex counter::_mutex;

} // namespace mongodb
} // namespace ops
