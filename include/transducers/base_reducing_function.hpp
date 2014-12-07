#pragma once

#include <type_traits>

#include "transducers/type_traits.hpp"
#include "transducers/reduction_wrapper.hpp"

namespace transducers {
    namespace toolbox {
        template<typename _ChildRf>
        class base_reducing_function
        {
        protected:
            using stored_rf = stored_argument_t<_ChildRf>;
            stored_rf m_rf;
        public:
            template<typename _Red, typename _Input>
            using result_type = decltype(std::declval<stored_rf>().step(std::declval<_Red>(), std::declval<_Input>()));

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