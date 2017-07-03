#pragma once

#include <random>
#include <limits>

namespace putils
{
    // Random int generator
    template<typename T, T start = 0, T end = std::numeric_limits<T>::max>
    inline T rand() noexcept
    {
        if constexpr (std::is_floating_point<T>::value)
                {
                        static std::random_device r;
                        static std::default_random_engine e(r());
                        std::uniform_real_distribution<T> d(start, end);
                        return d(e);
                }
        else
        {
            static std::random_device r;
            static std::default_random_engine e(r());
            static std::uniform_int_distribution<int> d;
            return start + d(e) % (end - start);
        }
    }
}
