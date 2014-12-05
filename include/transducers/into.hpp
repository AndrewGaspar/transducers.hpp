#pragma once

#include "transducers\transduce.hpp"

namespace transducers {
    namespace details {
        template<typename _It>
        class IteratorReducer
        {
        public:
            template<typename _In, typename _EsHa>
            _It step(_It iterator, _In&& input, _EsHa & hatch) const
            {
                *iterator = std::forward<_In>(input);
                return ++iterator;
            }

            _It complete(_It iterator) const
            {
                return iterator;
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
        return transduce(_begin, _end, out_begin, transducer, details::IteratorReducer<_It>());
    }

}