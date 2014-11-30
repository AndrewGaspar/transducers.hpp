#include "stdafx.h"
#include "CppUnitTest.h"

#include "common.h"

#include <transducers\interjecting.hpp>
#include <transducers\output.hpp>
#include <transducers\compose.hpp>

using namespace transducers;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace unit_tests
{
	TEST_CLASS(InterjectingTests)
	{
	public:
		
		TEST_METHOD(BetweenElements)
		{
            std::vector<std::string> fruits{ "banana", "apple", "cherry" };
            std::ostringstream stream;
            output_to(interjecting(", "), stream, fruits);
            std::string expected = "banana, apple, cherry";
            Assert::AreEqual(expected, stream.str());
		}

        TEST_METHOD(Nested)
        {
            std::string start = "||";
            std::ostringstream stream;
            output_to(compose(interjecting('!'), interjecting('^')), stream, start);
            Assert::AreEqual(std::string("|^!^|"), stream.str());
        }
	};
}