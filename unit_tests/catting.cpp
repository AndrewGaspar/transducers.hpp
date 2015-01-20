#include "stdafx.h"

#include <vector>
#include <list>

#include <transducers\catting.hpp>
#include <transducers\into_vector.hpp>
#include <transducers\mapcatting.hpp>

using namespace transducers;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace unit_tests
{
	TEST_CLASS(CattingTests)
	{
	public:
		
		TEST_METHOD(OutputsNumbersToOneToSix)
		{
            std::vector<std::vector<int>> aThing;
            aThing.emplace_back(std::vector<int>{ 1,2,3 });
            aThing.emplace_back(std::vector<int>{ 4,5,6 });

            auto result = into_vector(aThing, catting());

            Assert::AreEqual(std::vector<int>{1, 2, 3, 4, 5, 6}, result);
		}

        TEST_METHOD(MapCats)
        {
            struct ThingWithVector
            {
                std::vector<int> stuff;
            };

            std::vector<ThingWithVector> things;
            things.emplace_back(ThingWithVector{ std::vector<int>{1,2,3} });
            things.emplace_back(ThingWithVector{ std::vector<int>{4,5,6} });

            auto result = into_vector(things, mapcatting([](ThingWithVector const & thing) -> std::vector<int> const & {
                return thing.stuff;
            }));

            Assert::AreEqual(std::vector<int>{1, 2, 3, 4, 5, 6}, result);
        }
	};
}