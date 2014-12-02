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
        //static_assert(has_type<my_list, int>::value, "my_list has int");
	};
}