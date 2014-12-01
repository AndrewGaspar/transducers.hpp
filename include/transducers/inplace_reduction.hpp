#pragma once

#include "transducers/type_traits.hpp"
#include "transducers/conditional_move.hpp"

namespace transducers {
    template<typename _Red>
    class inplace_reduction
    {
    public:
        stored_argument_t<_Red> m_reduction;

        inplace_reduction(inplace_reduction const & cr) : m_reduction(cr.m_reduction) {};

        template<typename _RedP, REQUIRES(std::is_same<typename std::remove_reference<_Red>::type, typename std::remove_reference<_RedP>::type>::value)>
        inplace_reduction(_RedP&& collection) : m_reduction(std::forward<_RedP>(collection)) {}

        inplace_reduction(inplace_reduction&& cr) : m_reduction(conditional_move<!std::is_lvalue_reference<stored_argument_t<_Red>>::value>(cr.m_reduction)) {}

        

        inplace_reduction<_Red> & operator=(inplace_reduction<_Red> const & other) { return *this; }
    };
};