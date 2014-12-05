#pragma once

#include <type_traits>
#include <memory>

#include "transducers/type_traits.hpp"
#include "transducers/escape_hatch.hpp"
#include "transducers/conditional_move.hpp"

namespace transducers {

    template<typename _It, typename _Out, typename _Rf, typename _EsHa = nonatomic_escape_hatch>
    auto reduce(_It _begin, _It _end, _Out result, _Rf & rf, _EsHa eh = nonatomic_escape_hatch())
    {
        for (auto it = _begin; it != _end; it++)
        {
            result = rf.step(result, *it, eh);
            if (eh.should_terminate())
            {
                break;
            }
        }

        return rf.complete(result);
    }

    template<typename _It, typename _Rf, typename _EsHa = nonatomic_escape_hatch>
    auto reduce(_It _begin, _It _end, _Rf & rf, _EsHa eh = nonatomic_escape_hatch())
    {
        return reduce(_begin, _end, rf.init(), rf, std::move(eh));
    }

    template<typename _It, typename _Tr, typename _Re, typename _Out, typename _EsHa = nonatomic_escape_hatch>
    auto transduce(_It _begin, _It _end, _Out&& initial, _Tr const & transducer, _Re&& reducer, _EsHa hatch = nonatomic_escape_hatch())
    {
        auto rf = transducer.apply(std::forward<_Re>(reducer));

        return reduce(_begin, _end, std::forward<_Out>(initial), rf, std::move(hatch));
    }

    template<typename _InRa, typename _Out, typename _Tr, typename _Re,  typename _EsHa = nonatomic_escape_hatch(), REQUIRES(std::is_rvalue_reference<_InRa>::value)>
    auto transduce(_InRa&& input, _Out&& initial, _Tr const & transducer, _Re&& reducer, _EsHa hatch = nonatomic_escape_hatch())
    {
        return transduce(std::make_move_iterator(std::begin(input)),
            std::make_move_iterator(std::end(input)), std::forward<_Out>(initial), transducer,
            std::forward<_Re>(reducer), std::move(hatch));
    }

    template<typename _InRa, typename _Out, typename _Tr, typename _Re, typename _EsHa = nonatomic_escape_hatch()>
    auto transduce(_InRa& input, _Out&& initial, _Tr const & transducer, _Re&& reducer, _EsHa hatch = nonatomic_escape_hatch())
    {
        return transduce(std::begin(input), std::end(input), std::forward<_Out>(initial), transducer,
            std::forward<_Re>(reducer), std::move(hatch));
    }

    template<typename _It, typename _Tr, typename _Re, typename _EsHa = nonatomic_escape_hatch>
    auto transduce(_It _begin, _It _end, _Tr const & transducer, _Re&& reducer, _EsHa hatch = nonatomic_escape_hatch())
    {
        return transduce(_begin, _end, reducer.init(), transducer, std::forward<_Re>(reducer), std::move(hatch));
    }

    template<typename _InRa, typename _Tr, typename _Red, typename _EsHa = nonatomic_escape_hatch>
    auto transduce(_InRa&& input, _Tr const & transducer, _Red&& reducer, _EsHa hatch = nonatomic_escape_hatch())
    {
        return transduce(std::forward<_InRa>(input), reducer.init(), transducer, std::forward<_Red>(reducer), std::move(hatch));
    }
}