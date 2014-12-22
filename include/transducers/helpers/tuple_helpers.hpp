#pragma once

#include <tuple>

namespace transducers {
    namespace helpers {
        // http://stackoverflow.com/questions/16868129/how-to-store-variadic-template-arguments
        template <std::size_t... Is>
        struct index {};

        template <std::size_t N, std::size_t... Is>
        struct gen_seq : gen_seq<N - 1, N - 1, Is...> {};

        template <std::size_t... Is>
        struct gen_seq<0, Is...> : index<Is...>{};
    }
}