#pragma once

#include <type_traits>

#include "transducers/type_traits.hpp"

namespace transducers {
    namespace details {
        template<typename _Left, typename _Right>
        class ComposedTransducer
        {
            stored_argument_t<_Left> m_leftTransducer;
            stored_argument_t<_Right> m_rightTransducer;
        public:
            template<typename _InTyList>
            using output_typelist = typename std::remove_reference<_Right>::type::template
                output_typelist<typename std::remove_reference<_Left>::type::template
                    output_typelist<_InTyList>
                >;

            template<typename _LeftP, typename _RightP, REQUIRES(std::is_same<_Left, _LeftP>::value && std::is_same<_Right, _RightP>::value)>
            ComposedTransducer(_LeftP&& left, _RightP&& right) : m_leftTransducer(std::forward<_LeftP>(left)), m_rightTransducer(std::forward<_RightP>(right)) {}

            template<typename _Rf>
            auto apply(_Rf&& rf) const
            {
                return m_leftTransducer.apply(m_rightTransducer.apply(std::forward<_Rf>(rf)));
            }
        };
    }

    template<typename _Left, typename _Right>
    auto compose(_Left&& left, _Right&& right)
    {
        return details::ComposedTransducer<_Left, _Right>(std::forward<_Left>(left), std::forward<_Right>(right));
    }

    template <typename _Left, typename... TransducerTypes>
    auto compose(_Left&& left, TransducerTypes&&... rest_transducers)
    {
        return compose(std::forward<_Left>(left), compose(std::forward<TransducerTypes>(rest_transducers)...));
    }
}