#pragma once

#include <iterator>

#include "transducers/into.hpp"

namespace transducers {
    template<typename output_type, typename _Tr = std::nullptr_t, typename _InRa = std::nullptr_t>
    auto into_back(_Tr&& transducer, _InRa&& input)
    {
        output_type col;
        into(std::forward<_Tr>(transducer), std::forward<_InRa>(input),
            std::back_inserter(col));
        return col;
    }

    template<typename output_type, typename _Tr, typename _InRa>
    stored_argument_t<output_type> into_back(_Tr&& transducer, _InRa&& input, output_type&& output)
    {
        stored_argument_t<output_type> result(std::forward<output_type>(output));
        into(std::forward<_Tr>(transducer), std::forward<_InRa>(input),
            std::back_inserter(output));
        return result;
    }
}