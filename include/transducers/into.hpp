#pragma once

#include "transducers\transduce.hpp"
#include "transducers\reduction_wrapper.hpp"

namespace transducers {
    namespace details {
        template<typename _It>
        class IteratorReducer
        {
        public:
            template<typename _Re, typename _In>
            _Re step(_Re _it, _In&& input) const
            {
                auto & iterator = unwrap(_it);
                *iterator = std::forward<_In>(input);
                return ++iterator;
            }

            template<typename _Red>
            _It complete(_Red iterator) const
            {
                return unwrap(iterator);
            }
        };
    }

    template<typename _InRa, typename _It, typename _Tr>
    _It into(_InRa&& input, _It output_iterator, _Tr const & transducer)
    {
        return transduce(
            std::forward<_InRa>(input),
            output_iterator,
            transducer,
            details::IteratorReducer<_It>());
    }

    template<typename _InIt, typename _OutIt, typename _Tr>
    _OutIt into(_InIt _begin, _InIt _end, _OutIt out_begin, _Tr const & transducer)
    {
        return transduce(_begin, _end, out_begin, transducer, details::IteratorReducer<_OutIt>());
    }

}