#include "stdafx.h"
#include <CppUnitTest.h>

#include "tostring_overloads.h"

#include <transducers\mapping.hpp>
#include <transducers\into.hpp>
#include <transducers\into_vector.hpp>

using namespace transducers;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace unit_tests
{		
	TEST_CLASS(Mapping)
	{
	public:
		
		TEST_METHOD(VectorRValueTransducer)
		{
            auto incremented = into_vector(
                mapping([](int x) { return x + 1; }),
                std::vector<int> { 2, 4, 6 });

            std::vector<int> expected{ 3, 5, 7 };

            Assert::AreEqual<std::vector<int>>(expected, incremented, L"Each item should be incremented by one.");
		}

        TEST_METHOD(VectorLValueTransducer)
        {
            auto incrementer = mapping([](int x) { return x + 1; });

            auto incremented = into_vector(incrementer, std::vector<int> { 2, 4, 6 });

            std::vector<int> expected{ 3,5,7 };

            Assert::AreEqual<std::vector<int>>(expected, incremented, L"Each item should be incremented by one.");
        }
	};
}