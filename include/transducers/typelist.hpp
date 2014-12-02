#pragma once

#include <type_traits>

namespace transducers {
    template<typename ... _Types>
    struct typelist;

    template<typename _H>
    struct typelist<_H>
    {
        static const size_t length = 1;
        using head = _H;
    };

    

    template<typename _H, typename... _Rest>
    struct typelist<_H, _Rest...>
    {
        using head = _H;
        using tail = typelist<_Rest...>;
        static const size_t length = tail::length + 1;
    };

    /*template<typename typelist, typename type>
    struct has_type : std::integral_constant<bool, std::is_same<typename typelist::head, type>::value || has_type<typename typelist::tail, type>>
    {

    };
    
    template<typename typelist, typename type>
    struct has_type : std::false_type
    {
    };*/

}