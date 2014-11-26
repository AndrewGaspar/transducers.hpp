#pragma once

#include <type_traits>

#define REQUIRES(...) typename std::enable_if<(__VA_ARGS__), int>::type = 0

namespace transducers {
    template<typename T>
    struct stored_argument
    {
        using type = typename std::remove_reference<T>::type;
    };

    template<typename T>
    struct stored_argument<T&>
    {
        using type = typename std::remove_reference<T>::type&;
    };

    template<typename T>
    using stored_argument_t = typename stored_argument<T>::type;
};