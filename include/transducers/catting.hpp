#pragma once

#include "base_reducing_function.hpp"
#include "reduction_wrapper.hpp"
#include "typelist.hpp"

namespace transducers {
    namespace details {
        template<typename _Rf>
        class CattingReducer : public toolbox::base_reducing_function<_Rf>
        {
            using brf = typename toolbox::template base_reducing_function<_Rf>;
        public:
            CattingReducer(_Rf&& reducer) : toolbox::base_reducing_function<_Rf>(std::move(reducer)) {}

            template<typename _Red, typename _In>
            auto step(_Red&& r, _In&& input)
            {
                using result_t = typename brf::template result_type<_Red, _In>;

                result_t reduction = std::forward<_Red>(r);

                for(auto & in : input)
                {
                    reduction = brf::m_rf.step(reduction, in);

                    if (is_reduced(reduction))
                    {
                        return reduction;
                    }
                }

                return reduction;
            }
        };

        class CattingTransducer
        {
            struct catting_functor
            {
            public:
                template<typename _In>
                auto operator()(_In&& input)
                {
                    return *input.begin();
                }
            };
        public:
            template<typename _InTl>
            using output_typelist = transform_typelist_with_functor<_InTl, catting_functor>;

            template<typename _Rdr>
            auto apply(_Rdr&& reducer) const
            {
                return CattingReducer<_Rdr>{ std::move(reducer) };
            }
        };
    }

    inline auto catting()
    {
        return details::CattingTransducer{};
    }
}