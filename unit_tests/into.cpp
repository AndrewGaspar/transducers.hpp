#include "stdafx.h"
#include "CppUnitTest.h"

#include <vector>
#include <list>

#include <transducers\into.hpp>
#include <transducers\filtering.hpp>

#include "tostring_overloads.h"

using namespace transducers;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace unit_tests
{
	TEST_CLASS(into)
	{
	public:
		
		TEST_METHOD(into_list)
		{
            std::vector<int> input{ 1,2,3,4,5 };
            auto odd_numbers =
                transducers::into<std::list<int>>(
                    filtering([](int x) { return x % 2 == 1; }),
                    input
                );

            std::list<int> odd{ 1,3,5 };

            Assert::AreEqual(odd, odd_numbers, L"Only odd numbers should be filtered into list.");
		}

	};
}