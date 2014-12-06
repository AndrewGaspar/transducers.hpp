#include "stdafx.h"
#include "CppUnitTest.h"

#include <transducers\reduction_wrapper.hpp>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace unit_tests
{
	TEST_CLASS(ReductionWrapperTests)
	{
	public:
		
		TEST_METHOD(IntIsNotReduced)
		{
            Assert::IsFalse(transducers::is_reduced(3), L"Ints are never in a 'reduced state'");
		}


        TEST_METHOD(ReductionSetIsReduced)
        {
            transducers::reduction_wrapper<int> wrapper(5);
            wrapper.set_reduced();
            Assert::IsTrue(transducers::is_reduced(wrapper), L"If a reduction_wrapper is set_reduced, it should be is_reduced");
        }
	};
}