///
/// \file counter.h
///
#pragma once

#include <mutex>
#include "pool.h"

namespace ops
{
    namespace mongodb
    {
        class counter
        {
        public:
            counter() = delete;

            static constexpr auto Increment = 500;

            static std::uint64_t next();
            static std::string generate_id();

        private:
            static std::mutex _mutex;
        };
    }
}
