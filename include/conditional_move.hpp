#pragma once

#include <utility>

namespace transducers {
    namespace details {
        template<bool test, typename type>
        using conditional_move_t = typename std::conditional<test, typename std::remove_reference<type>::type&&, typename std::remove_reference<type>::type&>::type;
    };

    template<bool should_move, typename value_type = std::nullptr_t>
    auto conditional_move(value_type&& value) -> details::conditional_move_t<should_move, value_type>
    {
        return (static_cast<details::conditional_move_t<should_move, value_type>>(value));
    }

    template<typename parent_type, typename value_type = std::nullptr_t>
    auto move_if_owned(value_type&& value) -> decltype(conditional_move<std::is_rvalue_reference<parent_type>::value>(value))
    {
        return conditional_move<std::is_rvalue_reference<parent_type>::value>(value);
    };
}