#include "stdafx.h"

#include <transducers\compose.hpp>
#include <transducers\into_vector.hpp>

using namespace transducers;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace unit_tests
{
	TEST_CLASS(ComposeTests)
	{
	public:
		
		TEST_METHOD(IncrementAndDecrement)
		{
            std::vector<int> input{ 1,2,3 };
            auto output = into_vector(input, compose(incrementing, decrementing));
            Assert::AreEqual(input, output, L"The output should match the input.");
		}

	};
}