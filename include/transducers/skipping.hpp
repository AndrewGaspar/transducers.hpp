#pragma once

#include "transducers/base_reducing_function.hpp"
#include "transducers/reduction_wrapper.hpp"

namespace transducers {
    namespace details {
        template<typename _Rf>
        class SkippingReducingFunction : public toolbox::base_reducing_function<_Rf>
        {
            size_t m_numRemaining;
        public:
            static const bool wraps_reduction = true;

            SkippingReducingFunction(size_t numToTake, _Rf&& rf) : m_numRemaining(numToTake), toolbox::base_reducing_function<_Rf>(std::move(rf)) {}

            template<typename _Re, typename _In>
            auto step(_Re r, _In&& i) -> typename toolbox::base_reducing_function<_Rf>::template result_type<_Re, _In>
            {
                if (m_numRemaining > 0)
                {
                    --m_numRemaining;
                    return r;
                }

                return toolbox::base_reducing_function<_Rf>::m_rf.step(r, std::forward<_In>(i));
            }
        };

        class SkippingTransducer
        {
            size_t m_numberToSkip;
        public:
            template<typename _InTyList>
            using output_typelist = _InTyList;

            SkippingTransducer(size_t num) : m_numberToSkip(num) {}

            template<typename _Rf>
            auto apply(_Rf&& rf) const
            {
                return SkippingReducingFunction<_Rf>(m_numberToSkip, std::forward<_Rf>(rf));
            }
        };
    }

    // skips n inputs
    inline auto skipping(size_t numberToTake)
    {
        return details::SkippingTransducer(numberToTake);
    }
}