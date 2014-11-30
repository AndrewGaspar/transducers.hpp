#pragma once

#include <vector>
#include <iterator>

#include "transducers/transduce.hpp"
#include "transducers/inplace_reduction.hpp"
#include "transducers/into_it.hpp"

namespace transducers {

    template<typename output_type, typename _Tr = std::nullptr_t, typename _InRa = std::nullptr_t>
    auto into(_Tr&& transducer, _InRa&& input)
    {
        output_type col;
        into_it(std::forward<_Tr>(transducer), std::forward<_InRa>(input),
            std::back_inserter(col));
        return col;
    }

    template<typename output_type, typename _Tr, typename _InRa>
    stored_argument_t<output_type> into(_Tr&& transducer, _InRa&& input, output_type&& output)
    {
        stored_argument_t<output_type> result(std::forward<output_type>(output));
        into_it(std::forward<_Tr>(transducer), std::forward<_InRa>(input),
            std::back_inserter(output));
        return result;
    }

    template<typename transducer_t, typename _InRa>
    auto into_vector(transducer_t&& transducer, _InRa&& input)
    {
        using transducer_type = typename std::remove_reference<transducer_t>::type;
        using collection_type = typename std::remove_reference<_InRa>::type;
        using vector_value_type = typename transducer_type::template output_type<decltype(*(input.begin()))>;
        using vector_type = std::vector<vector_value_type>;

        vector_type v;
        into_it(std::forward<transducer_t>(transducer),
            std::forward<_InRa>(input),
            std::back_inserter(v));
        return v;
    }
}