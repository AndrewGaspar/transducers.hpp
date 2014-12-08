#include "stdafx.h"

#include <transducers\inserting_after_each.hpp>
#include <transducers\into_vector.hpp>
#include <transducers\output_to_string.hpp>
#include <transducers\taking.hpp>
#include <transducers\compose.hpp>

using namespace transducers;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace unit_tests
{
	TEST_CLASS(InsertingAfterEachTests)
	{
	public:
		
		TEST_METHOD(EachNumberLabeled)
		{
            Assert::AreEqual(std::vector<int> { 1, 2, 2, 2, 3, 2, 4, 2, 5, 2 }, 
                into_vector(std::vector<int> { 1, 2, 3, 4, 5 }, inserting_after_each(2)));
		}

        TEST_METHOD(InsertingAfterRespectsTermination)
        {
            Assert::AreEqual(std::vector<int>{1, 2, 2, 2, 3},
                into_vector(std::vector<int>{1, 2, 3, 4, 5}, compose(inserting_after_each(2), taking(5))));
        }

        TEST_METHOD(SupportsHeterogeneousTypes)
        {
            Assert::AreEqual(std::string{ "1,2,3," }, output_to_string(inserting_after_each(','), std::vector<int>{1, 2, 3}));
        }
	};
}