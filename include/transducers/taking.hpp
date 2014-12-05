#pragma once

#include "transducers/base_reducing_function.hpp"

namespace transducers {
    namespace details {
        template<typename _Rf>
        class TakingReducingFunction : public toolbox::base_reducing_function<_Rf>
        {
            size_t m_numRemaining;
        public:
            TakingReducingFunction(size_t numToTake, _Rf&& rf) : m_numRemaining(numToTake), toolbox::base_reducing_function<_Rf>(std::move(rf)) {}

            template<typename _Re, typename _In, typename _EsHa>
            _Re step(_Re r, _In&& i, _EsHa & hatch)
            {
                auto&& reduction = toolbox::base_reducing_function<_Rf>::m_rf.step(std::forward<_Re>(r), std::forward<_In>(i), hatch);
                m_numRemaining--;

                if (m_numRemaining <= 0)
                {
                    hatch.request_termination();
                }

                return reduction;
            }
        };

        class TakingTransducer
        {
            size_t m_numberToTake;
        public:
            template<typename _InTyList>
            using output_typelist = _InTyList;

            TakingTransducer(size_t num) : m_numberToTake(num) {}

            template<typename _Rf>
            auto apply(_Rf&& rf) const
            {
                return TakingReducingFunction<_Rf>(m_numberToTake, std::forward<_Rf>(rf));
            }
        };
    }

    // will always produce at least one input, even if you provide a value <= 0
    inline auto taking(size_t numberToTake)
    {
        return details::TakingTransducer(numberToTake);
    }
}