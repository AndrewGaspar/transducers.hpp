#pragma once

#include "transducers/reduction_wrapper.hpp"
#include "transducers/type_traits.hpp"
#include "transducers/typelist.hpp"
#include "transducers/base_reducing_function.hpp"

namespace transducers {
    namespace details {
        template<typename _Ins, typename _Rf>
        class InsertingAfterReducingFunction : public toolbox::base_reducing_function<_Rf>
        {
            _Ins const & m_insertion;
            bool skipped_first = false;
        public:
            InsertingAfterReducingFunction(_Ins const & insertion, _Rf&& rf) : m_insertion(insertion), toolbox::base_reducing_function<_Rf>(std::move(rf)) {}

            template<typename _Red, typename _Input>
            auto step(_Red r, _Input&& i)
            {
                auto reduction = toolbox::base_reducing_function<_Rf>::m_rf.step(r, std::forward<_Input>(i));

                if (transducers::is_reduced(reduction))
                {
                    return reduction;
                }

                return toolbox::base_reducing_function<_Rf>::m_rf.step(reduction, m_insertion);
            }
        };

        template<typename _Ins>
        class InsertingAfterTransducer
        {
            _Ins m_insertion;
        public:
            template<typename _InTyList>
            using output_typelist = extend_typelist<_InTyList, _Ins>;

            template<typename _InsP>
            InsertingAfterTransducer(_InsP&& insertion) : m_insertion(std::forward<_InsP>(insertion)) {}

            template<typename _Rf>
            auto apply(_Rf&& rf) const
            {
                return InsertingAfterReducingFunction<_Ins, _Rf>(m_insertion, std::forward<_Rf>(rf));
            }
        };
    }

    template<typename _Ins>
    auto inserting_after_each(_Ins&& insertion)
    {
        return details::InsertingAfterTransducer<_Ins>(std::forward<_Ins>(insertion));
    }
}