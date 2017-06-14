#pragma once

#include <algorithm>

namespace putils
{
    template<typename Container, typename Val>
    void erase(Container &container, Val &&val)
    {
        container.erase(std::find(container.begin(), container.end(), FWD(val)));
    }
}