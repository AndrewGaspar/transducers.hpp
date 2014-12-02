#pragma once

#include <type_traits>

namespace transducers {
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

    namespace details {
        template<typename _tl, typename _v = bool>
        struct remove_duplicates : _tl
        {

        };

        template<typename _tl>
        struct remove_duplicates<_tl,
            typename std::enable_if<
                has_tail<_tl>::value && has_type<typename _tl::tail, typename _tl::head>::value,
                bool
            >::type
        > : remove_duplicates<typename _tl::tail>
        {

        };

        template<typename _tl>
        struct remove_duplicates<_tl,
            typename std::enable_if <
                has_tail<_tl>::value && !has_type<typename _tl::tail, typename _tl::head>::value,
                bool
            >::type
        >
        {
            using head = typename _tl::head;
            using tail = typename remove_duplicates<typename _tl::tail>;
            static const size_t length = tail::length + 1;
        };

        template<typename... _Types>
        struct _raw_typelist;

        template<typename _H>
        struct _raw_typelist<_H>
        {
            static const size_t length = 1;
            using head = _H;
        };

        template<typename _H, typename... _Rest>
        struct _raw_typelist<_H, _Rest...>
        {
            using head = _H;
            using tail = _raw_typelist<_Rest...>;
            static const size_t length = tail::length + 1;
        };
    }

    template<typename... _Types>
    struct typelist : details::remove_duplicates<details::_raw_typelist<_Types...>> {};


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

    template<typename subset, typename superset, typename _v = bool>
    struct is_typelist_subset_of : std::false_type
    {
    };

    template<typename subset, typename superset>
    struct is_typelist_subset_of<
        subset,
        superset,
        typename std::enable_if<
            subset::length <= superset::length
            && has_type<superset, typename subset::head>::value
            && has_tail<subset>::value
            && is_typelist_subset_of<typename subset::tail, superset>::value,
            bool
        >::type
    >: std::true_type
    {};

    template<typename subset, typename superset>
    struct is_typelist_subset_of<
        subset,
        superset,
        typename std::enable_if<
            subset::length <= superset::length
            && !has_tail<subset>::value
            && has_type<superset, typename subset::head>::value,
            bool
        >::type
    >: std::true_type
    {};

    template<typename _left, typename _right>
    struct are_typelists_equivalent : std::integral_constant<bool, _left::length == _right::length
        && is_typelist_subset_of<_left, _right>::value
        && is_typelist_subset_of<_right, _left>::value>
    {
    };

    template<typename _tl>
    struct typelist_length : std::integral_constant<size_t, _tl::length>
    {

    };

    template<typename _tl, size_t _n>
    struct nth_type
    {
        using type = typename nth_type<typename _tl::tail, _n-1>::type;
    };

    template<typename _tl>
    struct nth_type<_tl, 0>
    {
        using type = typename _tl::head;
    };

    namespace details {

        template<typename _tl, typename _tr, typename _v = bool>
        struct _private_transform_typelist
        {
            using head = typename _tr::template transform<typename _tl::head>::type;
            static const size_t length = 1;
        };

        template<typename _tl, typename _tr>
        struct _private_transform_typelist<_tl, _tr,
            typename std::enable_if<
            has_tail<_tl>::value,
            bool
            >::type
        >
        {
            using head = typename _tr::template transform<typename _tl::head>::type;
            using tail = typename _private_transform_typelist<typename _tl::tail, _tr>;
            static const size_t length = tail::length + 1;
        };
    }

    template<typename _tl, typename _tr>
    struct transform_typelist : details::remove_duplicates<details::_private_transform_typelist<_tl, _tr>> {};
}