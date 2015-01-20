#pragma once

#include "transducers\reduction_wrapper.hpp"
#include "transducers\helpers\tuple_helpers.hpp"

#include <tuple>
#include <memory>

namespace transducers {
    namespace details {
        
        template<typename Rdr>
        class BaseReceivingReducer
        {
        protected:
            Rdr m_reducer;
        public:
            BaseReceivingReducer(Rdr&& reducer) : m_reducer(std::move(reducer)) {}

            auto init()
            {
                return m_reducer.init();
            }

            template<typename Re, typename In>
            auto step(Re reduction, In&& input)
            {
                return m_reducer.step(reduction, std::forward<In>(input));
            }
        };

        // This reducer is used for the first n-1 reducers - the completion should be ignored because only the
        // final completion is forwarded to the next reducer in line
        template<typename Rdr>
        class ReceivingReducer : public BaseReceivingReducer<Rdr>
        {
        public:
            ReceivingReducer(Rdr reducer) : BaseReceivingReducer<Rdr>(std::move(reducer)) {}

            template<typename Re>
            auto complete(Re&& reduction)
            {
                return std::forward<Re>(reduction);
            }
        };

        // This is used for the last reducer in the list - it receiving the completion call will result in the
        // next reducer receiving that completion call.
        template<typename Rdr>
        class FinalReceivingReducer : public BaseReceivingReducer<Rdr>
        {
        public:
            FinalReceivingReducer(Rdr reducer) : BaseReceivingReducer<Rdr>(std::move(reducer)) {}

            template<typename Re>
            auto complete(Re reduction)
            {
                return this->m_reducer.complete(reduction);
            }
        };

        template<typename... Rdrs>
        class MultiReducer
        {
            std::tuple<Rdrs...> m_reducers;
            
            template<typename Re, typename In, typename Rdr>
            auto recursive_step(Re&& reduction, In&& input, Rdr & reducer)
            {
                return reducer.step(std::forward<Re>(reduction), std::forward<In>(input));
            }

            template<typename Re, typename In, typename Rdr, typename... Rest>
            auto recursive_step(Re&& reduction, In&& input, Rdr & reducer, Rest&... rest)
            {
                using first_result_type = decltype(reducer.step(reduction, input));
                using rest_result_type = decltype(recursive_step(reduction, std::forward<In>(input), rest...));

                using result_type = typename std::conditional<
                    is_reduction_wrapper<first_result_type>::value ||
                    is_reduction_wrapper<rest_result_type>::value,
                    wrapped_t<first_result_type>,
                    first_result_type>::type;

                result_type _result = reduction;

                // do not forward input to step function because it can't
                // be consumed until the final reducer gets its hands on it
                _result = reducer.step(_result, input);
                if (is_reduced(_result))
                {
                    return _result;
                }

                return recursive_step(std::move(_result), std::forward<In>(input), rest...);
            }

            template<typename Re, typename In, std::size_t... Is>
            auto _step(Re&& reduction, In&& input, helpers::index<Is...>)
            {
                return recursive_step(std::forward<Re>(reduction), std::forward<In>(input), std::get<Is>(m_reducers)...);
            }

            template<typename Re, typename In>
            auto _step(Re&& reduction, In&& input)
            {
                return _step(std::forward<Re>(reduction), std::forward<In>(input), helpers::gen_seq<sizeof...(Rdrs)>{});
            }

            template<typename Re, typename Rdr>
            auto recursive_complete(Re&& reduction, Rdr & reducer)
            {
                return reducer.complete(std::forward<Re>(reduction));
            }

            template<typename Re, typename Rdr, typename... Rest>
            auto recursive_complete(Re&& reduction, Rdr & reducer, Rest & ... reducers)
            {
                return recursive_complete(reducer.complete(std::forward<Re>(reduction)), reducers...);
            }

            template<typename Re, std::size_t... Is>
            auto _complete(Re&& reduction, helpers::index<Is...>)
            {
                return recursive_complete(std::forward<Re>(reduction), std::get<Is>(m_reducers)...);
            }

            template<typename Re>
            auto _complete(Re&& reduction)
            {
                return _complete(std::forward<Re>(reduction), helpers::gen_seq<sizeof...(Rdrs)>{});
            }
        public:
            MultiReducer(std::tuple<Rdrs...>&& reducers) : m_reducers(std::move(reducers))
            {

            }

            auto init()
            {
                return std::get<0>(m_reducers).init();
            }

            template<typename Re, typename In>
            auto step(Re reduction, In&& input)
            {
                return _step(reduction, std::forward<In>(input));
            }

            template<typename Re>
            auto complete(Re&& reduction)
            {
                return _complete(std::forward<Re>(reduction));
            }
        };

        template<typename... Rdrs>
        auto construct_multireducer(std::tuple<Rdrs...>&& reducers) 
        {
            return MultiReducer<Rdrs...> { std::move(reducers) };
        }

        template<typename Rdr>
        class SharedReducer
        {
            std::shared_ptr<Rdr> m_reducer;
        public:
            SharedReducer(Rdr reducer) : m_reducer(std::make_shared<Rdr>(std::move(reducer))) {}

            auto init() -> decltype(auto)
            {
                return m_reducer->init();
            }

            template<typename Re, typename In>
            auto step(Re&& reduction, In&& input) -> decltype(auto)
            {
                return m_reducer->step(std::forward<Re>(reduction), std::forward<In>(input));
            }

            template<typename Re>
            auto complete(Re&& reduction) -> decltype(auto)
            {
                return m_reducer->complete(std::forward<Re>(reduction));
            }
        };

        template<typename... Ts>
        class MultiTransducer
        {
            std::tuple<Ts...> m_transducers;

            template<typename _Rdr, typename _Tdr>
            auto apply_reducer(_Rdr const & reducer, _Tdr const & transducer) const
            {
                return std::make_tuple(transducer.apply(FinalReceivingReducer<_Rdr>(reducer)));
            }

            template<typename _Rdr, typename _Tr, typename... _Trs>
            auto apply_reducer(_Rdr const & reducer, _Tr const & transducer, _Trs&... transducers) const
            {
                return std::tuple_cat(std::make_tuple(transducer.apply(ReceivingReducer<_Rdr>(reducer))), apply_reducer(reducer, transducers...));
            }

            template<typename _Rdr, std::size_t... _Is>
            auto apply_reducer_to_transducers(_Rdr const & reducer, helpers::index<_Is...>) const
            {
                return apply_reducer(reducer, std::get<_Is>(m_transducers)...);
            }

            template<typename _InList, typename _Tr>
            using _output_tl = typename std::decay<_Tr>::type::template output_typelist<_InList>;

        public:
            template<typename _InTyList>
            using output_typelist = concat_typelist<_output_tl<_InTyList, Ts>...>;

            template<typename... _Ts>
            MultiTransducer(_Ts&&... transducers) :
                m_transducers(std::forward<_Ts>(transducers)...)
            {
            }

            template<typename Rdr>
            auto apply(Rdr&& reducer) const
            {
                return construct_multireducer(apply_reducer_to_transducers(SharedReducer<Rdr>{ std::forward<Rdr>(reducer) }, helpers::gen_seq<sizeof...(Ts)>{}));
            }
        };
    }

    template<typename... Ts>
    auto multitransducing(Ts&&... transducers)
    {
        return details::MultiTransducer<Ts...>(std::forward<Ts>(transducers)...);
    }
}