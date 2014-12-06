#pragma once

#include "transducers/reduction_wrapper.hpp"
#include "transducers/type_traits.hpp"
#include "transducers/typelist.hpp"
#include "transducers/base_reducing_function.hpp"

namespace transducers {
    namespace details {
        template<typename _IntT, typename _Rf>
        class InterjectionReducingFunction : public toolbox::base_reducing_function<_Rf>
        {
            _IntT const & m_interjection;
            bool skipped_first = false;
        public:
            InterjectionReducingFunction(_IntT const & interjection, _Rf&& rf) : m_interjection(interjection), toolbox::base_reducing_function<_Rf>(std::move(rf)) {}

            template<typename _Red, typename _Input>
            auto step(_Red r, _Input&& i)
            {
                if (skipped_first)
                {
                    auto reduction = 
                        toolbox::base_reducing_function<_Rf>
                            ::m_rf.step(
                                r, 
                                m_interjection);

                    if (transducers::is_reduced(reduction))
                    {
                        return reduction;
                    }

                    return toolbox::base_reducing_function<_Rf>::m_rf.step(
                        reduction, 
                        std::forward<_Input>(i));
                }

                skipped_first = true;
                return toolbox::base_reducing_function<_Rf>::m_rf.step(std::forward<_Red>(r), std::forward<_Input>(i));
            }
        };

        template<typename _IntT>
        class InterjectionTransducer
        {
            _IntT m_interjection;
        public:
            template<typename _InTyList>
            using output_typelist = extend_typelist<_InTyList, _IntT>;

            template<typename _IntTP>
            InterjectionTransducer(_IntTP&& interjection) : m_interjection(std::forward<_IntTP>(interjection)) {}

            template<typename _Rf>
            auto apply(_Rf&& rf) const
            {
                return InterjectionReducingFunction<_IntT, _Rf>(m_interjection, std::forward<_Rf>(rf));
            }
        };
    }

    template<typename _IntT>
    auto interjecting(_IntT&& interjection)
    {
        return details::InterjectionTransducer<_IntT>(std::forward<_IntT>(interjection));
    }
}