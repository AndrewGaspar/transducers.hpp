#pragma once

#include <iterator>

#include "transducers/into.hpp"

namespace transducers {
    template<typename output_type, typename _InRa = std::nullptr_t, typename _Tr = std::nullptr_t>
    auto into_back(_InRa&& input, _Tr&& transducer)
    {
        output_type col;
        into(std::forward<_InRa>(input),
            std::back_inserter(col), std::forward<_Tr>(transducer));
        return col;
    }

    template<typename _InRa, typename output_type, typename _Tr>
    stored_argument_t<output_type> into_back(_InRa&& input, output_type&& output, _Tr&& transducer)
    {
        stored_argument_t<output_type> result(std::forward<output_type>(output));
        into(std::forward<_InRa>(input),
            std::back_inserter(result), std::forward<_Tr>(transducer));
        return result;
    }
}