#pragma once

#include <type_traits>

#include "type_traits.hpp"

namespace transducers {
    namespace details {
        template<typename _Left, typename _Right>
        struct are_types_pipelinable : 
            std::integral_constant<bool, 
                std::remove_reference<_Left>::type::has_one_to_one_output_type &&
                std::remove_reference<_Right>::type::has_one_to_one_output_type>
        { };

        struct non_pipelinable
        {
            static const bool has_one_to_one_output_type = false;
        };

        template<typename _Left, typename _Right>
        struct pipelinable
        {
            static const bool has_one_to_one_output_type = true;

            template<typename input_type>
            using output_type =
                typename std::remove_reference<_Right>::type::template
                output_type<typename std::remove_reference<_Left>::type::template
                output_type<typename std::remove_reference<input_type>::type>
                >;
        };

        template<typename _Left, typename _Right>
        using pipelinable_t = typename std::conditional<are_types_pipelinable<_Left, _Right>::value, pipelinable<_Left, _Right>, non_pipelinable>::type;

        template<typename _Left, typename _Right>
        class ComposedTransducer : public pipelinable_t<_Left, _Right>
        {
            stored_argument_t<_Left> m_leftTransducer;
            stored_argument_t<_Right> m_rightTransducer;
        public:

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