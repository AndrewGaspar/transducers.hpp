#include "stdafx.h"

#include <transducers\beginning.hpp>
#include <transducers\into_vector.hpp>
#include <transducers\compose.hpp>
#include <transducers\taking.hpp>

using namespace transducers;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace unit_tests
{
	TEST_CLASS(BeginningTests)
	{
	public:
		
		TEST_METHOD(InsertsAtBeginning)
		{
            std::vector<int> input{1, 2, 3};
            auto with_zero = into_vector(input, beginning(0));
            Assert::AreEqual(std::vector<int>{0, 1, 2, 3}, with_zero);
		}

        TEST_METHOD(RespectsTermination)
        {
            std::vector<int> input{ 1,2,3 };
            auto only_zero = into_vector(input, compose(beginning(0), taking(1)));
            Assert::AreEqual(std::vector<int>{0}, only_zero);
        }
	};
}