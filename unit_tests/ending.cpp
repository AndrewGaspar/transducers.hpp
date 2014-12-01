#include "stdafx.h"
#include "CppUnitTest.h"

#include <vector>
#include <string>

#include <transducers\ending.hpp>
#include <transducers\output_to_string.hpp>
#include <transducers\taking.hpp>
#include <transducers\compose.hpp>

using namespace transducers;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace unit_tests
{
	TEST_CLASS(EndingTests)
	{
	public:
		
		TEST_METHOD(Exclamation)
		{
            std::vector<std::string> boring{ "boring" };
            auto not_boring = output_to_string(ending('!'), boring);
            Assert::AreEqual("boring!", not_boring.c_str());
		}

	};
}