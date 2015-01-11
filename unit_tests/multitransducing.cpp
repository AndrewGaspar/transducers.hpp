#include "stdafx.h"

#include <transducers\mapping.hpp>
#include <transducers\multitransducing.hpp>
#include <transducers\output.hpp>

using namespace std;
using namespace std::literals::string_literals;
using namespace transducers;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace unit_tests
{
    struct Student
    {
        string name;
        int grade;

        Student(string name, int grade) : name(move(name)), grade(grade) {}
    };

	TEST_CLASS(MultitransducingTests)
	{
	public:
		
		TEST_METHOD(MapsMultiplePropertiesOfStruct)
		{
            vector<Student> students {
                {"Harry",93},
                {"Jed",75},
                {"Beth",98},
                {"Sue",68}
            };

            stringstream stream;
            output_to(multitransducing(
                mapping([](Student const & student) -> string const & { return student.name; }),
                mapping([](Student const &) { return ','; }),
                mapping([](Student const & student) { return student.grade; }),
                mapping([](Student const &) { return '\n'; })
            ), stream, students);

            Assert::AreEqual("Harry,93\nJed,75\nBeth,98\nSue,68\n"s, stream.str());
		}

	};
}