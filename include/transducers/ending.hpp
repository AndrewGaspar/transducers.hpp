#pragma once

#include "transducers/base_reducing_function.hpp"
#include "transducers/escape_hatch.hpp"
#include "transducers/typelist.hpp"
#include "transducers/reduction_wrapper.hpp"

namespace transducers {
    namespace details {
        template<typename _BooT, typename _Rf>
        class BookendReducingFunction : public toolbox::base_reducing_function<_Rf>
        {
            _BooT const & m_bookend;
            bool has_seen_completion = false;
        public:
            BookendReducingFunction(_BooT const & bookend, _Rf&& rf) : m_bookend(bookend), toolbox::base_reducing_function<_Rf>(std::move(rf)) {}

            template<typename _Red, typename _Input>
            auto step(_Red r, _Input&& i)
            {
                auto&& reduction = toolbox::base_reducing_function<_Rf>::m_rf.step(std::forward<_Red>(r), std::forward<_Input>(i));
                has_seen_completion = transducers::is_reduced(reduction);
                return reduction;
            }

            template<typename _Red>
            auto complete(_Red&& r)
            {
                if (has_seen_completion)
                {
                    return toolbox::base_reducing_function<_Rf>::m_rf.complete(std::forward<_Red>(r));
                }

                return toolbox::base_reducing_function<_Rf>::m_rf.complete(
                    toolbox::base_reducing_function<_Rf>::m_rf.step(std::forward<_Red>(r), m_bookend)
                );
            }
        };

        template<typename _BooT>
        class BookendTransducer
        {
            _BooT m_bookend;
        public:
            template<typename _InTyList>
            using output_typelist = extend_typelist<_InTyList, _BooT>;

            template<typename _BooTP>
            BookendTransducer(_BooTP&& bookend) : m_bookend(std::forward<_BooTP>(bookend)) {}

            template<typename _Rf>
            auto apply(_Rf&& rf) const
            {
                return BookendReducingFunction<_BooT, _Rf>(m_bookend, std::forward<_Rf>(rf));
            }
        };
    }

    template<typename _BooT>
    auto ending(_BooT&& bookend)
    {
        return details::BookendTransducer<_BooT>(std::forward<_BooT>(bookend));
    }
}