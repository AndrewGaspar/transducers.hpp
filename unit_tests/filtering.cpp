#include "stdafx.h"

#include <vector>

#include <transducers\filtering.hpp>
#include <transducers\mapping.hpp>
#include <transducers\compose.hpp>
#include <transducers\into.hpp>
#include <transducers\into_vector.hpp>

using namespace transducers;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace unit_tests
{
	TEST_CLASS(FilteringTests)
	{
	public:
		
		TEST_METHOD(NoEvens)
		{
            std::vector<int> input{ 1,2,3,4,5 };
            auto selecting_odds = filtering([](int x) { return x % 2 == 1; });
            input.erase(into(input, input.begin(), selecting_odds), input.end());

            std::vector<int> expected { 1,3,5 };
            Assert::AreEqual(expected, input, L"Evens erased");
		}

        TEST_METHOD(PointersAreBoolable)
        {
            std::vector<std::unique_ptr<int>> ptrs;
            ptrs.emplace_back(new int(1));
            ptrs.emplace_back(nullptr);
            ptrs.emplace_back(new int(3));

            auto selecting_non_null = filtering([](std::unique_ptr<int> & x) { return x.get(); });
            auto unwrapping = mapping([](std::unique_ptr<int>& x) { return *x; });

            auto nonNull = into_vector(compose(selecting_non_null, unwrapping), std::move(ptrs));

            std::vector<int> expected{ 1,3 };
            Assert::AreEqual(expected, nonNull);
        }
	};
}