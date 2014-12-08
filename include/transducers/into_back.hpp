#pragma once

#include <iterator>

#include "transducers/into.hpp"

namespace transducers {
    template<typename output_type, typename _InIt = std::nullptr_t, typename _Tr = std::nullptr_t>
    auto into_back(_InIt beg, _InIt end, _Tr&& transducer)
    {
        output_type col;
        into(beg, end, std::back_inserter(col), std::forward<_Tr>(transducer));
        return col;
    }

    template<typename output_type, typename _InRa = std::nullptr_t, typename _Tr = std::nullptr_t>
    auto into_back(_InRa&& input, _Tr&& transducer)
    {
        output_type col;
        into(std::forward<_InRa>(input),
            std::back_inserter(col), std::forward<_Tr>(transducer));
        return col;
    }
}