#pragma once

#include "transducers/type_traits.hpp"
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

            template<typename _Red, typename _Input, typename _EsHa>
            stored_argument_t<_Red> step(_Red&& r, _Input&& i, _EsHa & reduced)
            {
                if (skipped_first)
                {
                    auto&& reduction = 
                        toolbox::base_reducing_function<_Rf>
                            ::m_rf.step(
                                std::forward<_Red>(r), 
                                m_interjection, 
                                reduced);

                    if (reduced.should_terminate())
                    {
                        return reduction;
                    }

                    return toolbox::base_reducing_function<_Rf>::m_rf.step(
                        std::forward<_Red>(reduction), 
                        std::forward<_Input>(i), 
                        reduced);
                }

                skipped_first = true;
                return toolbox::base_reducing_function<_Rf>::m_rf.step(std::forward<_Red>(r), std::forward<_Input>(i), reduced);
            }
        };

        template<typename _IntT>
        class InterjectionTransducer
        {
            _IntT m_interjection;
        public:
            static const bool has_one_to_one_output_type = false;

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