#pragma once

#include <type_traits>
#include <memory>

#include "transducers/type_traits.hpp"
#include "transducers/escape_hatch.hpp"
#include "transducers/conditional_move.hpp"

namespace transducers {

    template<typename ReducingFunctor, typename Reduction, typename input_range, typename _EsHa>
    auto reduce(ReducingFunctor & rf, Reduction result, input_range&& input, _EsHa eh)
    {
        Reduction reduction = std::move(result);

        for (auto&& x : input)
        {
            reduction = rf.step(reduction, x, eh);
            if (eh.should_terminate())
            {
                break;
            }
        }

        return rf.complete(reduction);
    }

    template<typename _Tr, typename _Red, typename Reduction, typename _InRa, typename _EsHa>
    auto transduce(_Tr const & transducer, _Red&& reducer, Reduction&& result, _InRa&& input, _EsHa&& hatch)
    {
        auto rf = transducer.apply(std::forward<_Red>(reducer));
        
        return reduce(rf, std::forward<Reduction>(result), std::forward<_InRa>(input), std::forward<_EsHa>(hatch));
    }

    template<typename _Tr, typename _Red, typename _InRa, typename _EsHa = nonatomic_escape_hatch>
    auto transduce(_Tr const & transducer, _Red&& reducer, _InRa&& input, _EsHa hatch = nonatomic_escape_hatch())
    {
        return transduce(transducer, std::forward<_Red>(reducer), reducer.init(), std::forward<_InRa>(input), std::move(hatch));
    }
}