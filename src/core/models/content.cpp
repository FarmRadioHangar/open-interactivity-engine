#include "content.h"
#include <bsoncxx/builder/basic/kvp.hpp>
#include "rep.h"

using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::basic::make_document;

namespace core
{

content::content(const nlohmann::json& j)
  : ops::mongodb::model<content>{}
{
    _title = j.at("title");

    if (j.end() != j.find("id")) {
        _id = j.at("id");
    }

    const auto& reps = j.find("reps");

    if (j.end() != reps) {
        for (const auto& j : *reps) {
            for (const auto& j_rep : j) {
                _reps.emplace_back(core::rep{j_rep});
            }
        }
    }
}

bsoncxx::builder::basic::document content::get_builder() const
{
    bsoncxx::builder::basic::document builder{};

    builder.append(kvp("title", _title));

    if (_id.has_value()) {
        builder.append(kvp("id", _id.value()));
    }

    builder.append(kvp("reps", [this](bsoncxx::builder::basic::sub_document sub_builder) {
        for (const auto& rep : _reps) {
            sub_builder.append(kvp(rep.format(), [this, &rep](bsoncxx::builder::basic::sub_document sub_sub_builder) {
                sub_sub_builder.append(kvp(rep.language(), rep.builder().extract()));
            }));
        }
    }));

    return builder;
}

} // namespace core
