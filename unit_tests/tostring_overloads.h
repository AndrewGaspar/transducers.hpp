#pragma once

#include <CppUnitTest.h>
#include <vector>

#include <transducers\interject.hpp>
#include <transducers\output.hpp>

template<> static
std::wstring
Microsoft::VisualStudio::CppUnitTestFramework::ToString<std::vector<int>>(const std::vector<int>& t)
{
    using namespace transducers;

    std::wstringstream stream;
    output_to(interject(','), stream, t);
    return stream.str();
}