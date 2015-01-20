#pragma once

#include "compose.hpp"
#include "mapping.hpp"
#include "catting.hpp"

namespace transducers {
    template<typename _Functor>
    inline auto mapcatting(_Functor&& f)
    {
        return compose(mapping(std::forward<_Functor>(f)), catting());
    }
}