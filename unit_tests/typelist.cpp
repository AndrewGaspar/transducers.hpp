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

	};
}