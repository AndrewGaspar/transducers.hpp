#pragma once

#include <type_traits>

#include "transducers\type_traits.hpp"

namespace transducers {
    template<typename... _Types>
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

    template<typename T, typename V = bool>
    struct has_tail : std::false_type {};

    template<typename T>
    struct has_tail<T,
        typename std::enable_if<
            !std::is_same<typename T::tail, void>::value,
            bool
        >::type
    > : std::true_type
    {
        using type = typename T::tail;
    };

    template<typename typelist, typename type, typename V = bool>
    struct has_type : std::false_type {};

    template<typename typelist, typename type>
    struct has_type<typelist, type, 
        typename std::enable_if<
            std::is_same<
                typename typelist::head,
                type
            >::value
            || 
            has_type<typename typelist::tail, type>::value,
            bool
        >::type
    > : std::true_type
    {

    }; 
    
    template<typename typelist, typename type>
    struct has_type<typelist, type,
        typename std::enable_if<
        !has_tail<typelist>::value
        &&
        std::is_same<
            typename typelist::head,
            type
        >::value,
        bool
        >::type
    > : std::true_type
    {

    };

    template<typename _tl, typename _ty, typename _v = bool>
    struct extend_typelist
    {
        using head = _ty;
        using tail = _tl;
        static const size_t length = tail::length + 1;
    };

    template<typename _tl, typename _ty>
    struct extend_typelist<_tl, _ty, typename std::enable_if<
        has_type<_tl, _ty>::value,
        bool
    >::type
    > : _tl
    {
    };
}