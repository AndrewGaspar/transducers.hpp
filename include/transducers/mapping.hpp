#pragma once

#include "transducers/type_traits.hpp"
#include "transducers/base_reducing_function.hpp"

namespace transducers {

    namespace details {
        template<typename _MapFu, typename _Rf>
        class MappingReductionFunction : public toolbox::base_reducing_function<_Rf>
        {
            _MapFu const & f;
        public:
            MappingReductionFunction(_MapFu const & f, _Rf&& rf) : f(f), toolbox::base_reducing_function<_Rf>(std::move(rf)) {}

            template<typename Reduction, typename Input, typename _EsHa>
            stored_argument_t<Reduction> step(Reduction&& r, Input&& i, _EsHa & reduced)
            {
                return toolbox::base_reducing_function<_Rf>::m_rf.step(std::forward<Reduction>(r), f(std::forward<Input>(i)), reduced);
            }
        };

        template<typename _MapFu>
        class MappingTransducer
        {
            stored_argument_t<_MapFu> _f;
        public:
            static const bool has_one_to_one_output_type = true;

            template<typename _InTy>
            using output_type = decltype(std::declval<decltype(_f)>()((std::declval<_InTy>())));

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
