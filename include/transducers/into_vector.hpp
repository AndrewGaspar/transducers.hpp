#pragma once

#include <vector>
#include <iterator>

#include "transducers/into_it.hpp"
#include "transducers/typelist.hpp"

namespace transducers {


    template<typename transducer_t, typename _InRa>
    auto into_vector(transducer_t&& transducer, _InRa&& input)
    {
        using transducer_type = typename std::remove_reference<transducer_t>::type;
        using collection_type = typename std::remove_reference<_InRa>::type;
        using input_type = typename std::remove_reference<collection_type::value_type>::type;
        using output_typelist = typename transducer_type::template output_typelist<typelist<input_type>>;

        static_assert(output_typelist::length == 1, "The transducer can only output a single type to get vector output type deduction.");

        using vector_output_type = typename nth_type<output_typelist, 0>::type;
        using vector_type = std::vector<vector_output_type>;

        vector_type v;
        into_it(std::forward<transducer_t>(transducer),
            std::forward<_InRa>(input),
            std::back_inserter(v));
        return v;
    }
}