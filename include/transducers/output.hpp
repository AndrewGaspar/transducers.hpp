#pragma once

#include <type_traits>

#include "transducers/transduce.hpp"
#include "transducers/type_traits.hpp"
#include "transducers/reduction_wrapper.hpp"

namespace transducers {
    namespace details {
        template<typename _Os>
        class StreamReducer
        {
            stored_argument_t<_Os> m_outStream;
        public:
            using ostream_type = _Os;

            template<typename Reduction, typename Input, 
                REQUIRES(std::is_same<std::reference_wrapper<ostream_type>, typename reduction_type<Reduction>::type>::value)>
            Reduction step(Reduction reduction, Input&& input)
            {
                unwrap(reduction).get() << std::forward<Input>(input);
                return reduction;
            }

            template<typename T>
            std::reference_wrapper<ostream_type> complete(T reduction)
            {
                return unwrap(reduction);
            }
        };
    }

    template<typename _Tr, typename _Ra, typename _Os>
    auto output_to(_Tr&& transducer, _Os & output_stream, _Ra&& input)
    {
        return transduce(std::forward<_Ra>(input), std::reference_wrapper<_Os>(output_stream), std::forward<_Tr>(transducer), details::StreamReducer<_Os>());
    }
};