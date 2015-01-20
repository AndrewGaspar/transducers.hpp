#pragma once

#include "transducers/base_reducing_function.hpp"
#include "transducers/escape_hatch.hpp"
#include "transducers/typelist.hpp"
#include "transducers/reduction_wrapper.hpp"

namespace transducers {
    namespace details {
        template<typename _Beg, typename _Rf>
        class BeginningReducer : public toolbox::base_reducing_function<_Rf>
        {
            _Beg m_beginning;
            bool has_begun = false;
        public:
            BeginningReducer(_Beg const & beginning, _Rf&& rf) : m_beginning(beginning), toolbox::base_reducing_function<_Rf>(std::move(rf)) {}

            template<typename _Red, typename _Input>
            auto step(_Red r, _Input&& i)
            {
                using brf = typename toolbox::template base_reducing_function<_Rf>;

                if (!has_begun)
                {
                    has_begun = true;

                    auto reduction = brf::m_rf.step(r, m_beginning);
                    if (transducers::is_reduced(reduction))
                    {
                        return reduction;
                    }
                    return brf::m_rf.step(reduction, std::forward<_Input>(i));
                }

                return brf::m_rf.step(r, std::forward<_Input>(i));
            }
        };

        template<typename _Beg>
        class BeginningTransducer
        {
            _Beg m_beginning;
        public:
            template<typename _InTyList>
            using output_typelist = extend_typelist<_InTyList, _Beg>;

            template<typename _BegT>
            BeginningTransducer(_BegT&& beginning) : m_beginning(std::forward<_BegT>(beginning)) {}

            template<typename _Rf>
            BeginningReducer<_Beg, _Rf> apply(_Rf&& rf) const
            {
                return BeginningReducer<_Beg, _Rf>(m_beginning, std::forward<_Rf>(rf));
            }
        };
    }

    template<typename _Beg>
    auto beginning(_Beg&& beginning)
    {
        return details::BeginningTransducer<_Beg>(std::forward<_Beg>(beginning));
    }
}