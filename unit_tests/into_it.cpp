#include "stdafx.h"
#include "CppUnitTest.h"

#include "common.h"

#include <vector>
#include <iterator>

#include <transducers\into_it.hpp>
#include <transducers\bookend.hpp>

using namespace transducers;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace unit_tests
{
	TEST_CLASS(IntoIteratorTests)
	{
	public:
		
		TEST_METHOD(BackInserter)
		{
            std::vector<int> input{ 1,2,3 };
            std::vector<int> output;

            into_it(incrementing, input, std::back_inserter(output));

            std::vector<int> expected{ 2,3,4 };
            Assert::AreEqual(expected, output, L"input incremented");
		}

        TEST_METHOD(ReuseVector)
        {
            std::vector<int> input{ 1,3,5 };
            into_it(incrementing, input, input.begin());

            std::vector<int> expected{ 2,4,6 };
            Assert::AreEqual(expected, input, L"Input mutated");
        }

        TEST_METHOD(IntoSet)
        {
            std::vector<int> input{ -2, -1, 0, 1, 2 };
            auto squaring = mapping([](int x) { return x * x; });
            std::set<int> squares;
            transducers::into_it(squaring, input, std::inserter(squares, squares.begin()));

            std::set<int> expected{ 0,1,4 };

            Assert::AreEqual(expected, squares, L"Only single instance of squared");
        }
	};
}