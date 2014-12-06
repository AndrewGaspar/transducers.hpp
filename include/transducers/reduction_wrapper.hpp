#pragma once

#include <type_traits>

namespace transducers {
    template<typename _Reduction>
    class reduction_wrapper
    {
    public:
        using reduction_type = _Reduction;
        static const bool _is_reduction_wrapper = true;
    private:
        bool m_isReduced = false;
        reduction_type m_reduction;
    public:

        reduction_wrapper(reduction_type reduction) : m_reduction(std::move(reduction)) {}

        reduction_type & reduction()
        {
            return m_reduction;
        }

        bool is_reduced() const
        {
            return m_isReduced;
        }

        void set_reduced()
        {
            m_isReduced = true;
        }
    };

    template<typename _Reduction>
    reduction_wrapper<_Reduction> fully_reduced(_Reduction&& r)
    {
        reduction_wrapper<_Reduction> wrapper(unwrap(std::forward<_Reduction>(r)));
        wrapper.set_reduced();
        return std::move(wrapper);
    }

    template<typename T>
    constexpr bool is_reduced(T const &) { return false; }

    template<typename _Reduction>
    bool is_reduced(reduction_wrapper<_Reduction> const & reduction) { return reduction.is_reduced(); }

    template<typename _Rf, typename V = bool>
    struct wraps_reduction : std::false_type {};

    template<typename _Rf>
    struct wraps_reduction<_Rf, typename std::enable_if<_Rf::wraps_reduction, bool>::type> : std::true_type{};

    template<typename T, typename V = bool>
    struct is_reduction_wrapper : std::false_type {};

    template<typename T>
    struct is_reduction_wrapper<T, typename std::enable_if<T::_is_reduction_wrapper, bool>::type> : std::true_type{};

    template<typename T, typename V = bool>
    struct reduction_type
    {
        using type = T;
    };

    template<typename T>
    struct reduction_type<T, typename std::enable_if<is_reduction_wrapper<T>::value, bool>::type>
    {
        using type = typename T::reduction_type;
    };

    template<typename T>
    using reduction_type_t = typename reduction_type<T>::type;

    template<typename T>
    struct wrapped : std::conditional<is_reduction_wrapper<T>::value, T, reduction_wrapper<T>>
    {

    };

    template<typename T>
    using wrapped_t = typename wrapped<T>::type;

    template<typename T>
    T&& unwrap(T&& t) { return t; }

    template<typename _Reduction>
    _Reduction& unwrap(reduction_wrapper<_Reduction> & reduction) { return reduction.reduction(); }
}