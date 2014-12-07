#include "stdafx.h"

#include <transducers\interjecting.hpp>
#include <transducers\output.hpp>
#include <transducers\compose.hpp>
#include <transducers\taking.hpp>

using namespace transducers;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace unit_tests
{
	TEST_CLASS(InterjectingTests)
	{
	public:
        std::vector<std::string> fruits;

        InterjectingTests()
        {
            fruits = { "banana", "apple", "cherry" };
        }

		TEST_METHOD(BetweenElements)
		{
            std::ostringstream stream;
            output_to(interjecting(", "), stream, fruits);
            std::string expected = "banana, apple, cherry";
            Assert::AreEqual(expected, stream.str());
		}

        TEST_METHOD(RespectsTermination)
        {
            std::ostringstream stream;
            output_to(compose(interjecting(','), taking(4)), stream, fruits);
            std::string expected = "banana,apple,";
            Assert::AreEqual(expected, stream.str());
        }

        TEST_METHOD(RespectsTermination2)
        {
            std::ostringstream stream;
            output_to(compose(interjecting(','), taking(3)), stream, fruits);
            std::string expected = "banana,apple";
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