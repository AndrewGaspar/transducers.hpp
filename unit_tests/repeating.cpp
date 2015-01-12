#include "stdafx.h"

#include <transducers\output_to_string.hpp>
#include <transducers\repeating.hpp>
#include <transducers\compose.hpp>
#include <transducers\taking.hpp>

using namespace transducers;
using namespace std::literals::string_literals;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace unit_tests
{
	TEST_CLASS(RepeatingTests)
	{
	public:
		
		TEST_METHOD(AAAAAAA)
		{
            char A [] {'A'};
            auto AAAAAAA = output_to_string(repeating(7), A);
            Assert::AreEqual("AAAAAAA"s, AAAAAAA);
		}

        TEST_METHOD(RepeatingSupportsEarlyTermination)
        {
            char B []{ "B" };
            auto BBBB = output_to_string(compose(repeating(7), taking(4)), B);
            Assert::AreEqual("BBBB"s, BBBB);
        }
	};
}