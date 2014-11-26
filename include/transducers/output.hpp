#pragma once

#include "transducers/transduce.hpp"
#include "transducers/type_traits.hpp"
#include "transducers/inplace_reduction.hpp"

namespace transducers {
    namespace details {
        template<typename _Os>
        class StreamReducer
        {
            stored_argument_t<_Os> m_outStream;
        public:
            using ostream_type = typename std::remove_reference<_Os>::type;

            StreamReducer(_Os&& output_stream) : m_outStream(std::forward<_Os>(output_stream)) {}

            inplace_reduction<ostream_type&> init()
            {
                return inplace_reduction<ostream_type&>(m_outStream);
            }

            template<typename Input, typename _EsHa>
            inplace_reduction<ostream_type&> & step(inplace_reduction<ostream_type&> & reduction, Input&& input, _EsHa&)
            {
                reduction.m_reduction << std::forward<Input>(input);
                return reduction;
            }

            std::reference_wrapper<ostream_type> complete(inplace_reduction<ostream_type&> & reduction)
            {
                return reduction.m_reduction;
            }
        };
    }

    template<typename _Tr, typename _Ra, typename _Os>
    auto output_to(_Tr&& transducer, _Os&& output_stream, _Ra&& input)
    {
        return transduce(std::forward<_Tr>(transducer), details::StreamReducer<_Os>(output_stream), std::forward<_Ra>(input), nonatomic_escape_hatch());
    }
};