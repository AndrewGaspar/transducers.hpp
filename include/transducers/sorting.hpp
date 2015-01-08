#pragma once

#include <algorithm>
#include <functional>
#include <utility>
#include <vector>

#include "transducers/typelist.hpp"
#include "transducers/base_reducing_function.hpp"
#include "transducers/reduction_wrapper.hpp"

namespace transducers {
    namespace details {
        template<typename _Heap, typename _Pr, typename _Rf>
        class SortingReducer : public toolbox::base_reducing_function<_Rf>
        {
            _Pr const & m_comparison;
            std::vector<_Heap> m_heap;
        public:
            SortingReducer(_Pr const & comparison, _Rf&& reducer) : 
                m_comparison(comparison), 
                toolbox::base_reducing_function<_Rf>(std::move(reducer)) 
            {

            }

            template<typename _Re, typename _In>
            auto step(_Re&& reduction, _In&& input)
            {
                m_heap.push_back(std::forward<_In>(input));
                return std::forward<_Re>(reduction);
            }

            template<typename _Re>
            auto complete(_Re reduction)
            {
                using namespace std::placeholders;

                auto & reducer = toolbox::base_reducing_function<_Rf>::m_rf;

                using result_type = decltype(reducer.step(reduction, std::declval<_Heap>()));
                result_type result = reduction;

                // The heap is organized with the "largest" element out front, but we want to get the smallest element first.
                // This reverse the arguments of the comparison function so that the "biggest" element according to std::heap
                // is the smallest.
                // This is done so that we don't have to sort the entire list before delivering elements. That way a user of 
                // the transducer gets better performance if they don't take all of the results.
                auto reverse_comparator = std::bind(m_comparison, _2, _1);

                // Use reverse iterators so the elements popped from the heap
                // are moved to the front of the vector.
                // This will lead the buffer being sorted in memory.
                // Hopefully will help with shared buffers later.
                auto begin = m_heap.rbegin();
                auto end = m_heap.rend();

                // constructs the heap
                std::make_heap(begin, end, reverse_comparator);

                // The heaped portion of the buffer slowly shrinks over time
                while (begin != end)
                {
                    // move min element to front of vector
                    std::pop_heap(begin, end, reverse_comparator);

                    // min element is no longer part of the heap
                    end--;

                    // end now points to the current minimum
                    result = reducer.step(result, std::move(*end));

                    if (is_reduced(result))
                    {
                        break;
                    }
                }

                return reducer.complete(std::move(result));
            }
        };

        template<typename _In, typename _Pr>
        class SortingTransducer
        {
            _Pr m_comparison;
        public:
            template<typename _InTyList>
            using output_typelist = _InTyList;

            template<typename _PrT>
            SortingTransducer(_PrT&& comparison) : 
                m_comparison(std::forward<_PrT>(comparison)) 
            {

            }

            template<typename _Rf>
            auto apply(_Rf&& rf) const
            {
                return SortingReducer<_In, _Pr, _Rf>(m_comparison, std::forward<_Rf>(rf));
            }
        };
    }

    template<typename _In, typename _Pr = void>
    auto sorting(_Pr&& comp)
    {
        return details::SortingTransducer<_In, _Pr>{std::forward<_Pr>(comp)};
    }

    template<typename _In>
    auto sorting()
    {
        return sorting<_In>(std::less<_In>{});
    }
}