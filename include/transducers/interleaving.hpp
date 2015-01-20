#pragma once

#include "transducers/reduction_wrapper.hpp"
#include "transducers/type_traits.hpp"
#include "transducers/typelist.hpp"
#include "transducers/base_reducing_function.hpp"

namespace transducers {
    namespace details {
        template<typename _Ins, typename _Rf>
        class InterleavingReducer : public toolbox::base_reducing_function<_Rf>
        {
            _Ins const & m_insertion;
            bool first_not_yet_inserted = true;
        public:
            InterleavingReducer(_Ins const & insertion, _Rf&& rf) : m_insertion(insertion), toolbox::base_reducing_function<_Rf>(std::move(rf)) {}

            template<typename _Red, typename _Input>
            auto step(_Red r, _Input&& i)
            {
                using brf = typename toolbox::template base_reducing_function<_Rf>;
                using result_t = typename brf::template result_type<_Red, _Input>;

                result_t reduction = std::forward<_Red>(r);

                if (first_not_yet_inserted)
                {
                    first_not_yet_inserted = false;

                    reduction = brf::m_rf.step(reduction, m_insertion);

                    if (is_reduced(reduction))
                    {
                        return reduction;
                    }
                }

                reduction = brf::m_rf.step(reduction, std::forward<_Input>(i));

                if (is_reduced(reduction))
                {
                    return reduction;
                }

                return brf::m_rf.step(reduction, m_insertion);
            }
        };

        template<typename _Ins>
        class InterleavingTransducer
        {
            _Ins m_insertion;
        public:
            template<typename _InTyList>
            using output_typelist = extend_typelist<_InTyList, _Ins>;

            template<typename _InsP>
            InterleavingTransducer(_InsP&& insertion) : m_insertion(std::forward<_InsP>(insertion)) {}

            template<typename _Rf>
            auto apply(_Rf&& rf) const
            {
                return InterleavingReducer<_Ins, _Rf>(m_insertion, std::forward<_Rf>(rf));
            }
        };
    }

    template<typename _Ins>
    auto interleaving(_Ins&& insertion)
    {
        return details::InterleavingTransducer<_Ins>(std::forward<_Ins>(insertion));
    }
}