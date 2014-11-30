#include "stdafx.h"
#include "CppUnitTest.h"

#include <vector>
#include <list>
#include <set>

#include <transducers\into.hpp>
#include <transducers\filtering.hpp>
#include <transducers\mapping.hpp>

#include "tostring_overloads.h"

using namespace transducers;

using Assert = Microsoft::VisualStudio::CppUnitTestFramework::Assert;

namespace unit_tests
{
	TEST_CLASS(IntoTests)
	{
	public:
		
		TEST_METHOD(IntoList)
		{
            std::vector<int> input{ 1,2,3,4,5 };
            auto odd_numbers =
                transducers::into<std::list<int>>(
                    filtering([](int x) { return x % 2 == 1; }),
                    input
                );

            std::list<int> odd{ 1,3,5 };

            Assert::AreEqual(odd, odd_numbers, L"Only odd numbers should be filtered into list.");
		}
        
        TEST_METHOD(IntoVector)
        {
            std::vector<int> input{ -5,-3,-1,1,3,5 };
            auto halving = mapping([](int x) { return float(x) / 2.0f; });
            auto halved = into_vector(halving, input);

            std::vector<float> expected{ -2.5f, -1.5f, -0.5f, 0.5f, 1.5f, 2.5f };

            Assert::AreEqual(expected, halved, L"Things halved.");
        }
	};
}