#pragma once

#include <vector>
#include <iterator>

#include "transducers/into_it.hpp"

namespace transducers {


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