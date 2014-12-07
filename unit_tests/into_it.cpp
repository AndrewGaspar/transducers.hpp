#include "stdafx.h"

#include <vector>
#include <iterator>

#include <transducers\into.hpp>

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

            into(input, std::back_inserter(output), incrementing);

            std::vector<int> expected{ 2,3,4 };
            Assert::AreEqual(expected, output, L"input incremented");
		}

        TEST_METHOD(ReuseVector)
        {
            std::vector<int> input{ 1,3,5 };
            into(input, input.begin(), incrementing);

            std::vector<int> expected{ 2,4,6 };
            Assert::AreEqual(expected, input, L"Input mutated");
        }

        TEST_METHOD(IntoSet)
        {
            std::vector<int> input{ -2, -1, 0, 1, 2 };
            auto squaring = mapping([](int x) { return x * x; });
            std::set<int> squares;
            transducers::into(input, std::inserter(squares, squares.begin()), squaring);

            std::set<int> expected{ 0,1,4 };

            Assert::AreEqual(expected, squares, L"Only single instance of squared");
        }
	};
}