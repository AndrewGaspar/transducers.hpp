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

    template<typename _Tr, typename _InRa, typename _It>
    _It into(_Tr&& transducer, _InRa&& input, _It output_iterator)
    {
        return transduce(
            std::forward<_InRa>(input),
            output_iterator,
            std::forward<_Tr>(transducer),
            details::IteratorReducer<_It>(),
            nonatomic_escape_hatch());
    }

}