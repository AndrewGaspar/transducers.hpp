#pragma once

#include <utility>

namespace transducers {
    namespace details {
        template<typename _Step>
        class GenericReducer
        {
            _Step m_stepFunction;
        public:
            template<typename _StepT>
            GenericReducer(_StepT&& stepFunction) : 
                m_stepFunction(std::forward<_StepT>(stepFunction)) {}

            template<typename _OutT, typename _InT>
            auto step(_OutT&& reduction, _InT&& input) const
            {
                return m_stepFunction(std::forward<_OutT>(reduction), std::forward<_InT>(input));
            }

            template<typename _OutT>
            auto const & complete(_OutT const & reduction) const
            {
                return reduction;
            }
        };
    }

    template<typename _Step>
    auto reducer(_Step&& reducingFunctor)
    {
        return details::GenericReducer<_Step>(std::forward<_Step>(reducingFunctor));
    }
}