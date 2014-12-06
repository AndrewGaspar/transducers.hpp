#pragma once

#include <utility>

namespace transducers {
    namespace details {
        template<typename _Out, typename _Step>
        class GenericReducer
        {
            _Out m_initialValue;
            _Step m_stepFunction;
        public:
            template<typename _OutT, typename _StepT>
            GenericReducer(_OutT&& initialValue, _StepT&& stepFunction) : 
                m_initialValue(std::forward<_OutT>(initialValue)),
                m_stepFunction(std::forward<_StepT>(stepFunction)) {}

            _Out init() { return std::move(m_initialValue); }

            template<typename _OutT, typename _InT>
            _Out step(_OutT&& reduction, _InT&& input) const
            {
                return m_stepFunction(std::forward<_OutT>(reduction), std::forward<_InT>(input));
            }

            _Out const & complete(_Out const & reduction) const
            {
                return reduction;
            }
        };
    }

    template<typename _Out, typename _Step>
    auto reducer(_Out&& initial, _Step&& reducingFunctor)
    {
        return details::GenericReducer<_Out, _Step>(std::forward<_Out>(initial), std::forward<_Step>(reducingFunctor));
    }
}