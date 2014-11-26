#pragma once

#include "transducers/transduce.hpp"
#include "transducers/inplace_reduction.hpp"

namespace transducers {
    namespace details {

        template<typename T>
        class CollectionReducer
        {
        public:
            using collection_type = typename std::remove_reference<T>::type;
            using input_type = typename collection_type::value_type;

            inplace_reduction<collection_type> init() const
            {
                return collection_type();
            }

            template<typename _Co, typename Input, typename _EsHa>
            inplace_reduction<_Co> & step(inplace_reduction<_Co> & reduction, Input&& input, _EsHa&) const
            {
                reduction.m_reduction.emplace_back(std::forward<Input>(input));
                return reduction;
            }

            template<typename _Co>
            _Co complete(inplace_reduction<_Co> & reduction) const
            {
                return conditional_move<!std::is_lvalue_reference<decltype(reduction.m_reduction)>::value>(reduction.m_reduction);
            }
        };
    }

    template<typename output_type, typename _Tr = std::nullptr_t, typename _InRa = std::nullptr_t>
    auto into(_Tr&& transducer, _InRa&& input)
    {
        return transduce(std::forward<_Tr>(transducer), details::CollectionReducer<output_type>(), std::forward<_InRa>(input), nonatomic_escape_hatch());
    }

    template<typename output_type, typename _Tr, typename _InRa>
    auto into(_Tr&& transducer, _InRa&& input, output_type&& output)
    {
        return transduce(std::forward<_Tr>(transducer), details::CollectionReducer<output_type>(),
            inplace_reduction<output_type>(std::forward<output_type>(output)), std::forward<_InRa>(input), nonatomic_escape_hatch());
    }

    template<typename transducer_t, typename _InRa>
    auto into_vector(transducer_t&& transducer, _InRa&& input)
    {
        using transducer_type = typename std::remove_reference<transducer_t>::type;
        using collection_type = typename std::remove_reference<_InRa>::type;
        using vector_value_type = typename transducer_type::template output_type<decltype(*(input.begin()))>;
        using vector_type = std::vector<vector_value_type>;

        return into<vector_type>(std::forward<transducer_t>(transducer), std::forward<_InRa>(input));
    }
}