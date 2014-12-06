#pragma once

#include "transducers/type_traits.hpp"
#include "transducers/reduction_wrapper.hpp"

namespace transducers {
    namespace toolbox {
        template<typename _ChildRf>
        class base_reducing_function
        {
        protected:
            stored_argument_t<_ChildRf> m_rf;
        public:
            template<typename _Re>
            using reduction_type = typename std::conditional<wraps_reduction<_Re>::value, reduction_wrapper<_Re>, _Re>::type;

            base_reducing_function(_ChildRf&& rf) : m_rf(std::move(rf)) {}

            auto init() { return m_rf.init(); }

            template<typename _Red>
            auto complete(_Red r)
            {
                return m_rf.complete(r);
            }
        };
    }
}