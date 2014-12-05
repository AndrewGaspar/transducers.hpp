#include "stdafx.h"
#include "CppUnitTest.h"

#include <transducers\reducer.hpp>
#include <transducers\mapping.hpp>

using namespace transducers;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace unit_tests
{
	TEST_CLASS(ReducerTests)
	{
	public:
		
		TEST_METHOD(Sum)
		{
            std::vector<int> input{ 1,2,3 };
            auto sum = reduce(input.begin(), input.end(),
                reducer(0, [](int acc, int val) { return acc + val; }));
            Assert::AreEqual(6, sum, L"Sum of inputs should be 6.");
		}

	};
}