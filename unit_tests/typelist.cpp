#include "stdafx.h"

#include <transducers\typelist.hpp>

using namespace transducers;

namespace unit_tests
{
	class TypelistAssertions
	{
	public:
        static_assert(transducers::typelist<int>::length == 1, "typelist with 1 element is length 1.");
        static_assert(transducers::typelist<int, float>::length == 2, "typelist with 2 elements is length 2.");
        static_assert(transducers::typelist<int, float, bool>::length == 3, "typelist length is calculated correctly");

        using my_list = transducers::typelist<int, float, bool>;
        static_assert(transducers::has_tail<my_list>::value, "my_list has tail");
        static_assert(!transducers::has_tail<my_list::tail::tail>::value, "my_list::tail::tail has no tail.");
        static_assert(has_type<my_list, int>::value, "my_list has int");
        static_assert(has_type<my_list, bool>::value, "my_list has bool");
        static_assert(!has_type<my_list, char>::value, "my_list does not have char");

        static_assert(my_list::length == extend_typelist<my_list, int>::length, "my_list extended with int should be the same as my_list");
        static_assert(my_list::length == extend_typelist<my_list, float>::length, "my_list extended with float should be the same as my_list");
        static_assert(my_list::length == extend_typelist<my_list, bool>::length, "my_list extended with bool should be the same as my_list");
        static_assert(my_list::length + 1 == extend_typelist<my_list, char>::length, "my_list extended with char should extend the length of my_list by 1");

        using short_list = transducers::typelist<int>;
        static_assert(short_list::length == 1, "1 element in short_list");
        static_assert(short_list::length == extend_typelist<short_list, int>::length, "short_list extended with int should be the same as short_list");
        static_assert(short_list::length + 1 == extend_typelist<short_list, char>::length, "short_list extended with char should extend short_list by 1");

        using float_int = transducers::typelist<float, int>;
        using int_bool_float = transducers::typelist<int, bool, float>;
        static_assert(is_typelist_subset_of<float_int, int_bool_float>::value, "float_int should be a subset of int_bool_float");
        static_assert(!is_typelist_subset_of<int_bool_float, float_int>::value, "int_bool_float is not a subset of float_int");
        static_assert(!are_typelists_equivalent<int_bool_float, float_int>::value, "int_bool_float is not equivalent to float_int");

        using float_int_bool = transducers::typelist<float, int, bool>;
        using int_bool_float = transducers::typelist<int, bool, float>;
        using bool_float_int = transducers::typelist<bool, float, int>;
        static_assert(are_typelists_equivalent<float_int_bool, int_bool_float>::value, "float_int_bool and int_bool_float are equivalent.");
        static_assert(are_typelists_equivalent<int_bool_float, bool_float_int>::value, "int_bool_float and bool_float_int are equivalent.");
        static_assert(are_typelists_equivalent<bool_float_int, float_int_bool>::value, "bool_float_int and float_int_bool are equivalent.");

        static_assert(std::is_same<typename nth_type<float_int_bool, 0>::type, float>::value, "0th type in float_int_bool is float.");
        static_assert(std::is_same<typename nth_type<float_int_bool, 1>::type, int>::value, "1st type in float_int_bool is int.");
        static_assert(std::is_same<typename nth_type<float_int_bool, 2>::type, bool>::value, "2nd type in float_int_bool is bool.");
	};
}