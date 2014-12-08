#pragma once

#include "transducers/type_traits.hpp"
#include "transducers/typelist.hpp"
#include "transducers/base_reducing_function.hpp"

namespace transducers {

    namespace details {
        template<typename _MapFu, typename _Rf>
        class MappingReductionFunction : public toolbox::base_reducing_function<_Rf>
        {
            _MapFu const & f;
        public:
            MappingReductionFunction(_MapFu const & f, _Rf&& rf) : f(f), toolbox::base_reducing_function<_Rf>(std::move(rf)) {}

            template<typename Reduction, typename Input>
            auto step(Reduction r, Input&& i)
            {
                return toolbox::base_reducing_function<_Rf>::m_rf.step(r, f(std::forward<Input>(i)));
            }
        };

        template<typename _MapFu>
        class MappingTransducer
        {
            stored_argument_t<_MapFu> _f;
        public:
            template<typename _InTyList>
            using output_typelist = typename transform_typelist_with_functor<_InTyList, typename std::remove_reference<_MapFu>::type>;

            template<typename _MapFuP,
                REQUIRES(std::is_same<_MapFu, _MapFuP>::value)>
                explicit MappingTransducer(_MapFuP&& f) : _f(std::forward<_MapFuP>(f)) {}

            template<typename _Red>
            auto apply(_Red&& rf) const
            {
                return MappingReductionFunction<_MapFu, _Red>(_f, std::forward<_Red>(rf));
            }
        };
    }

    template<typename _MapFu>
    auto mapping(_MapFu&& f)
    {
        return details::MappingTransducer<_MapFu>(std::forward<_MapFu>(f));
    };
}
