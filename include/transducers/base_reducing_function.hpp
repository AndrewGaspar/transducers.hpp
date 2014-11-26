#pragma once

#include "transducers/type_traits.hpp"

namespace transducers {
    namespace toolbox {
        template<typename _ChildRf>
        class base_reducing_function
        {
        protected:
            stored_argument_t<_ChildRf> m_rf;
        public:
            base_reducing_function(_ChildRf&& rf) : m_rf(std::move(rf)) {}

            auto init() { return m_rf.init(); }

            template<typename _Red>
            auto complete(_Red&& r)
            {
                return m_rf.complete(std::forward<_Red>(r));
            }
        };
    }
}