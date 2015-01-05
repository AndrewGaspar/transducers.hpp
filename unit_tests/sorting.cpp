#include "stdafx.h"

#include <vector>
#include <transducers\into.hpp>
#include <transducers\sorting.hpp>
#include <transducers\taking.hpp>
#include <transducers\compose.hpp>

using namespace transducers;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace unit_tests
{
	TEST_CLASS(SortingTests)
	{
	public:
		
		TEST_METHOD(SortsElements)
		{
            std::vector<int> my_nums{3, 5, 7, 2, 1, 9};
            transducers::into(my_nums.begin(), my_nums.end(), 
                my_nums.begin(), sorting<int>());
            Assert::AreEqual(my_nums, std::vector<int>{1, 2, 3, 5, 7, 9});
		}

        TEST_METHOD(SortsWithCustomComparator)
        {
            std::vector<int> my_nums{ 3,5,7,2,1,9 };
            transducers::into(my_nums.begin(), my_nums.end(), 
                my_nums.begin(), sorting<int>(std::greater<>()));
            Assert::AreEqual(my_nums, std::vector<int>{9, 7, 5, 3, 2, 1});
        }

        TEST_METHOD(PartialSorting)
        {
            std::vector<int> my_nums{ 9,2,5,1,8,15,8,6,12,-5,8,4,2,9 };
            auto end = transducers::into(my_nums.begin(), my_nums.end(), my_nums.begin(), 
                compose(sorting<int>(), taking(4)));
            Assert::AreEqual(std::vector<int>{my_nums.begin(), my_nums.begin() + 4},
                std::vector<int>{-5,1,2,2});
            Assert::IsFalse(std::is_sorted(my_nums.begin(), my_nums.end()));
        }
	};
}