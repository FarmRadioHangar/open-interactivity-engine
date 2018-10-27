#include "content.h"
#include <bsoncxx/builder/basic/kvp.hpp>
#include "rep.h"

using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::basic::make_document;

namespace ops
{

content_builder::content_builder(const nlohmann::json& j)
  : mongodb::bson::builder{j}
{
    append(kvp("title", std::string{j.at("title")}));

    if (j.end() != j.find("id")) {
        append(kvp("id", std::string{j.at("id")}));
    }

    const auto& reps = j.find("reps");

    if (j.end() != reps) {
        bsoncxx::builder::basic::document collection_builder{};
        for (nlohmann::json::const_iterator i = reps->begin(); i != reps->end(); ++i) {
            const auto& format = i.key();
            const nlohmann::json& languages = i.value();
            bsoncxx::builder::basic::document sub_collection_builder{};
            for (nlohmann::json::const_iterator j = languages.begin(); j != languages.end(); ++j) {
                sub_collection_builder.append(kvp(std::string{j.key()},
                      rep_builder{j.value()}.extract()));
            }
            collection_builder.append(kvp(std::string{format},
                  sub_collection_builder.extract()));
        }
        append(kvp("reps", collection_builder.extract()));
    }
}

} // namespace ops
