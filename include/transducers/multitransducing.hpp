#pragma once

#include "transducers\reduction_wrapper.hpp"
#include "transducers\helpers\tuple_helpers.hpp"

#include <tuple>

namespace transducers {
    namespace details {
        
        template<typename Rdr>
        class BaseReceivingReducer
        {
            Rdr & m_reducer;
        public:
            BaseReceivingReducer(Rdr & reducer) : m_reducer(reducer) {}

            template<typename Re, typename In>
            auto step(Re reduction, In&& input)
            {
                return m_reducer.step(reduction, std::forward<In>(input));
            }
        };

        template<typename Rdr>
        class ReceivingReducer : public BaseReceivingReducer<Rdr>
        {
        public:
            ReceivingReducer(Rdr & reducer) : BaseReceivingReducer<Rdr>(reducer) {}

            template<typename Re>
            auto complete(Re)
            {
                // do nothing
            }
        };

        template<typename Rdr>
        class FirstReceivingReducer : public ReceivingReducer<Rdr>
        {
        public:
            FirstReceivingReducer(Rdr & reducer) : ReceivingReducer<Rdr>(reducer) {}

            auto init()
            {
                return m_reducer.init();
            }
        };

        template<typename Rdr>
        class FinalReceivingReducer : public BaseReceivingReducer<Rdr>
        {
        public:
            FinalReceivingReducer(Rdr & reducer) : BaseReceivingReducer<Rdr>(reducer) {}

            template<typename Re>
            auto complete(Re reduction)
            {
                return m_reducer.complete(reduction);
            }
        };

        template<typename... Rdrs>
        class MultiReducer
        {
            std::tuple<Rdrs...> m_reducers;
            
            template<typename Re, typename In, typename Rdr>
            auto _step(Re reduction, In&& input, Rdr & reducer)
            {
                return reducer.step(reduction, std::forward<In>(input));
            }

            template<typename Re, typename In, typename Rdr, typename... Rdrs>
            auto recursive_step(Re reduction, In&& input, Rdr & reducer, Rdrs&... rest)
            {
                // do not forward input to step function because it can't
                // be consumed until the final reducer gets its hands on it
                auto reduced = reducer.step(reduction, input);
                if (is_reduced(reduced))
                {
                    return reduced;
                }

                return recursive_step(reduction, std::forward<In>(input), rest...);
            }

            template<typename Re, typename In, typename... Rdrs, std::size_t... Is>
            auto _step(Re reduction, In&& input, std::tuple<Rdrs...>& reducers, helpers::index<Is...>)
            {
                return recursive_step(re, std::forward<In>(input), std::get<Is>(reducers)...);
            }

            template<typename Re, typename In, typename... Rdrs>
            auto _step(Re reduction, In&& input, std::tuple<Rdrs...>& reducers)
            {
                return _step(reduction, std::forward<In>(input), reducers, helpers::gen_seq<sizeof...(Rdrs)>{});
            }

            template<typename Re, typename Rdr>
            auto recursive_complete(Re reduction, Rdr & reducer)
            {
                return reducer.complete(reduction);
            }

            template<typename Re, typename Rdr, typename... Rdrs>
            auto recursive_complete(Re reduction, Rdr & reducer, Rdrs & ... reducers)
            {
                return recursive_complete(reducer.complete(reduction), reducers...);
            }

            template<typename Re, typename... Rdrs, std::size_t... Is>
            auto _complete(Re reduction, std::tuple<Rdrs...> & reducers, helpers::index<Is...>)
            {
                return recursive_complete(reduction, std::get<Is>(reducers)...);
            }

            template<typename Re, typename... Rdrs>
            auto _complete(Re reduction, std::tuple<Rdrs...> & reducers)
            {
                return _complete(reduction, reducers, helpers::gen_seq<sizeof...(Rdrs)>{});
            }
        public:
            template<typename ...Args>
            MultiReducer(Args... reducers) : m_reducers(std::forward<Args>(reducers)...)
            {

            }

            auto init()
            {
                return std::get<0>(m_reducers).init();
            }

            template<typename Re, typename In>
            auto step(Re reduction, In&& input)
            {
                return _step(reduction, std::forward<In>(input), m_reducers);
            }

            template<typename Re>
            auto complete(Re reduction)
            {
                return _complete(reduction, m_reducers);
            }
        };

        template<typename... Rdrs>
        auto construct_multireducer(Rdrs&&... reducers) 
        {
            return MultiReducer<Rdrs>(std::forward(reducers)...);
        }

        template<typename... Ts>
        class MultiTransducer
        {
            Ts... m_transducers;

            template<typename Rdr>
            class ReducerApplier
            {
                Rdr & m_reducer;
            public:
                ReducerApplier(Rdr reducer) : m_reducer(reducer) {}

                template<typename Tr>
                auto apply_transducer(Tr const & transducer)
                {
                    return transducer.apply(m_reducer);
                }
            };

        public:

            MultiTransducer(Ts... transducers) : m_transducers(transducers)
            {

            }

            template<typename Rdr>
            auto apply(Rdr & reducer)
            {
                ReducerApplier applier{ reducer };

                return construct_multireducer(applier.apply_transducer(m_transducers)...);
            }
        };
    }

    template<typename... Ts>
    auto multitransducing(Ts... transducers)
    {
        return details::MultiTransducer<Ts>(transducers...);
    }
}