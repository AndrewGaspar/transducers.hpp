#include "stdafx.h"

#include <vector>
#include <string>

#include <transducers\ending.hpp>
#include <transducers\output_to_string.hpp>
#include <transducers\into_back.hpp>
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

        TEST_METHOD(EndingRespectsTermination)
        {
            std::vector<int> input{ 1,2,3 };
            auto subset = into_back<std::vector<int>>(input, compose(ending(4), taking(2)));
            std::vector<int> expected{ 1,2 };
            Assert::AreEqual(expected, subset);
        }

        TEST_METHOD(EndingAfterTaking)
        {
            std::vector<int> input{ 1,2,3 };
            auto subset = into_back<std::vector<int>>(input, compose(taking(2), ending(4)));
            std::vector<int> expected{ 1,2,4 };
            Assert::AreEqual(expected, subset);
        }
	};
}