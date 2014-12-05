#pragma once

#include <type_traits>

#include "transducers/transduce.hpp"
#include "transducers/type_traits.hpp"

namespace transducers {
    namespace details {
        template<typename _Os>
        class StreamReducer
        {
            stored_argument_t<_Os> m_outStream;
        public:
            using ostream_type = _Os;

            template<typename Input, typename _EsHa>
            std::reference_wrapper<ostream_type> step(std::reference_wrapper<ostream_type> reduction, Input&& input, _EsHa&)
            {
                reduction.get() << std::forward<Input>(input);
                return reduction;
            }

            std::reference_wrapper<ostream_type> complete(std::reference_wrapper<ostream_type> & reduction)
            {
                return reduction;
            }
        };
    }

    template<typename _Tr, typename _Ra, typename _Os>
    auto output_to(_Tr&& transducer, _Os & output_stream, _Ra&& input)
    {
        return transduce(std::forward<_Ra>(input), std::reference_wrapper<_Os>(output_stream), std::forward<_Tr>(transducer), details::StreamReducer<_Os>(), nonatomic_escape_hatch());
    }
};