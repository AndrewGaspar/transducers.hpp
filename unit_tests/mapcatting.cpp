#include "stdafx.h"

#include <transducers\mapcatting.hpp>
#include <transducers\output_to_string.hpp>
#include <transducers\interjecting.hpp>
#include <transducers\compose.hpp>

using namespace std::string_literals;
using namespace transducers;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace unit_tests
{
	TEST_CLASS(MapcattingTests)
	{
	public:
		
		TEST_METHOD(AppendsCharacters)
		{
            class PathSegment
            {
                std::vector<std::string> m_segments;
            public:
                PathSegment(std::initializer_list<std::string> segments) : m_segments(segments) {}

                std::vector<std::string> const & segments() const
                {
                    return m_segments;
                }
            };

            std::vector<PathSegment> strs{ { {"docs"}, {"cpp"} }, { {"Effective Modern C++.pdf"} } };
            auto path = output_to_string(compose(
                mapcatting([](PathSegment const & segment) -> std::vector<std::string> const & {
                    return segment.segments();
                }),
                interjecting('\\')), strs);
            Assert::AreEqual("docs\\cpp\\Effective Modern C++.pdf"s, path);
		}

	};
}