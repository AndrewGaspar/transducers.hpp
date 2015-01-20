#include "stdafx.h"

#include <transducers\interleaving.hpp>
#include <transducers\output_to_string.hpp>

using namespace transducers;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std::string_literals;

namespace unit_tests
{
	TEST_CLASS(InterleavingTests)
	{
	public:
		
		TEST_METHOD(VerticalBarInterleaving)
		{
            std::vector<int> input{ 1,2,3 };
            auto result = output_to_string(interleaving('|'), input);
            Assert::AreEqual("|1|2|3|"s, result);
		}

	};
}