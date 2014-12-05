#pragma once

#include <type_traits>
#include <memory>

#include "transducers/type_traits.hpp"
#include "transducers/escape_hatch.hpp"
#include "transducers/conditional_move.hpp"

namespace transducers {

    template<typename _It, typename _Out, typename _Rf, typename _EsHa>
    auto reduce(_It _begin, _It _end, _Out result, _Rf & rf, _EsHa eh)
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

    template<typename _InRa, typename _Tr, typename _Red, typename _Out,  typename _EsHa>
    auto transduce(_InRa&& input, _Tr const & transducer, _Red&& reducer, _Out&& result, _EsHa&& hatch)
    {
        auto rf = transducer.apply(std::forward<_Red>(reducer));
        
        return reduce(input.begin(), input.end(), std::forward<_Out>(result), rf, std::forward<_EsHa>(hatch));
    }

    template<typename _InRa, typename _Tr, typename _Red, typename _EsHa = nonatomic_escape_hatch>
    auto transduce(_InRa&& input, _Tr const & transducer, _Red&& reducer, _EsHa hatch = nonatomic_escape_hatch())
    {
        return transduce(std::forward<_InRa>(input), transducer, std::forward<_Red>(reducer), reducer.init(), std::move(hatch));
    }
}