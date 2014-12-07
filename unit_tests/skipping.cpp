#include "stdafx.h"

#include <vector>

#include <transducers\into_vector.hpp>
#include <transducers\skipping.hpp>

using namespace transducers;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace unit_tests
{
	TEST_CLASS(SkippingTests)
	{
	public:
		
		TEST_METHOD(SkipsFirst)
		{
            std::vector<int> input{ 2,4,6 };
            auto no_two = into_vector(input, skipping(1));
            std::vector<int> expected{ 4,6 };
            Assert::AreEqual(expected, no_two);
		}

        TEST_METHOD(SkipsNone)
        {
            std::vector<int> input{ 2,4,6 };
            auto no_skip = into_vector(input, skipping(0));
            Assert::AreEqual(input, no_skip);
        }

        TEST_METHOD(SkipsAll)
        {
            std::vector<int> input{ 2,4,6 };
            auto empty = into_vector(input, skipping(3));
            Assert::AreEqual(std::vector<int>(), empty);
        }
	};
}