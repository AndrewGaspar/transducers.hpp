#pragma once

#include <sstream>

#include "transducers/output.hpp"

namespace transducers {


    template<typename _Char = char, typename _Tr = nullptr_t, typename _Ra = nullptr_t>
    auto output_to_string(_Tr&& transducer, _Ra&& input)
    {
        std::basic_stringstream<_Char> stream;
        output_to(std::forward<_Tr>(transducer), stream, std::forward<_Ra>(input));
        return stream.str();
    }
}