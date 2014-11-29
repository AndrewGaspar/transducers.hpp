#include "stdafx.h"
#include "CppUnitTest.h"

#include "common.h"

#include <vector>
#include <iterator>

#include <transducers\into_it.hpp>

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

	};
}