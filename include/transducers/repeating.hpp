#pragma once

#include "transducers/base_reducing_function.hpp"
#include "transducers/taking.hpp"
#include "transducers/reduction_wrapper.hpp"

namespace transducers {
    namespace details {
        template<typename _Rdr>
        class RepeatingReducer : public toolbox::base_reducing_function<_Rdr>
        {
            size_t const m_repetitionsRemaining;
        public:
            RepeatingReducer(size_t repetitions, _Rdr&& reducer) : 
                m_repetitionsRemaining(repetitions),
                toolbox::base_reducing_function<_Rdr>(std::move(reducer)) {}

            template<typename _Re, typename _In>
            auto step(_Re&& reduction, _In&& input)
            {
                auto & reducer = toolbox::base_reducing_function<_Rdr>::m_rf;

                using result_type = decltype(reducer.step(std::forward<_Re>(reduction), std::declval<_In>()));
                result_type my_reduction = std::forward<_Re>(reduction);
                _In repeated = std::forward<_In>(input);

                for (auto i = 0; i < m_repetitionsRemaining; i++)
                {
                    my_reduction = reducer.step(my_reduction, repeated);

                    if (is_reduced(my_reduction))
                    {
                        break;
                    }
                }

                return my_reduction;
            }
        };

        class RepeatingTransducer
        {
            size_t m_repetitions;
        public:
            template<typename _InTyList>
            using output_typelist = _InTyList;

            RepeatingTransducer(size_t repetitions) : m_repetitions(repetitions) {}

            template<typename _Rdr>
            auto apply(_Rdr&& reducer) const
            {
                return RepeatingReducer<_Rdr>{m_repetitions, std::forward<_Rdr>(reducer)};
            }
        };
    }

    auto repeating(size_t repetitions)
    {
        return details::RepeatingTransducer{ repetitions };
    }
}