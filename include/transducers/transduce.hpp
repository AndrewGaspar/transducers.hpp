#pragma once

#include <type_traits>
#include <memory>

#include "transducers/type_traits.hpp"
#include "transducers/escape_hatch.hpp"
#include "transducers/conditional_move.hpp"
#include "transducers/reduction_wrapper.hpp"

namespace transducers {

    template<typename _It, typename _Out, typename _Rf>
    auto reduce(_It _begin, _It _end, _Out result, _Rf & rf)
    {
        using result_type = decltype(rf.step(result, *_begin));
        result_type _result = result;

        for (auto it = _begin; it != _end; ++it)
        {
            _result = rf.step(_result, *it);
            if (is_reduced(_result))
            {
                break;
            }
        }

        return rf.complete(_result);
    }

    template<typename _It, typename _Rf>
    auto reduce(_It _begin, _It _end, _Rf & rf)
    {
        return reduce(_begin, _end, rf.init(), rf);
    }

    template<typename _It, typename _Tr, typename _Re, typename _Out>
    auto transduce(_It _begin, _It _end, _Out initial, _Tr const & transducer, _Re&& reducer)
    {
        auto rf = transducer.apply(std::forward<_Re>(reducer));

        return reduce(_begin, _end, initial, rf);
    }

    template<typename _InRa, typename _Out, typename _Tr, typename _Re>
    auto transduce(_InRa&& input, _Out initial, _Tr const & transducer, _Re&& reducer)
    {
        stored_argument_t<_InRa> inputRange = std::forward<_InRa>(input);
        return transduce(std::begin(inputRange), std::end(inputRange), initial, transducer, std::forward<_Re>(reducer));
    }

    template<typename _It, typename _Tr, typename _Re>
    auto transduce(_It _begin, _It _end, _Tr const & transducer, _Re&& reducer)
    {
        return transduce(_begin, _end, reducer.init(), transducer, std::forward<_Re>(reducer));
    }

    template<typename _InRa, typename _Tr, typename _Red>
    auto transduce(_InRa&& input, _Tr const & transducer, _Red&& reducer)
    {
        return transduce(std::forward<_InRa>(input), reducer.init(), transducer, std::forward<_Red>(reducer));
    }
}