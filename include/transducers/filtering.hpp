#pragma once

#include "transducers/type_traits.hpp"
#include "transducers/base_reducing_function.hpp"

namespace transducers {

    namespace details {
        template<typename FilteringFunction, typename _Rf>
        class FilterReductionFunction : public toolbox::base_reducing_function<_Rf>
        {
        public:
            FilteringFunction f;

            FilterReductionFunction(FilteringFunction const & f, _Rf&& rf) : f(f), toolbox::base_reducing_function<_Rf>(std::move(rf)) {}

            template<typename Reduction, typename Input>
            auto step(Reduction r, Input&& i) -> result_type<Reduction, Input>
            {
                static_assert(std::is_convertible<decltype(f(i)), bool>::value, "f(i) must produce a type convertible to bool in filtering transducers.");

                return f(i) 
                    ? toolbox::base_reducing_function<_Rf>::m_rf.step(std::forward<Reduction>(r), std::forward<Input>(i))
                    : std::forward<Reduction>(r);
            }
        };

        template<typename _FilteringFunctor>
        class FilteringTransducer
        {
            stored_argument_t<_FilteringFunctor> _f;
        public:
            template<typename _InTyList>
            using output_typelist = _InTyList;

            template<typename FilteringFunction,
                REQUIRES(std::is_same<_FilteringFunctor, FilteringFunction>::value)>
            explicit FilteringTransducer(FilteringFunction&& f) : _f(std::forward<FilteringFunction>(f)) {}

            template<typename _Red>
            auto apply(_Red&& rf) const
            {
                return FilterReductionFunction<_FilteringFunctor, _Red>(_f, std::forward<_Red>(rf));
            }
        };
    }

    template<typename FilteringFunction>
    auto filtering(FilteringFunction&& f)
    {
        return details::FilteringTransducer<FilteringFunction>(std::forward<FilteringFunction>(f));
    };
}