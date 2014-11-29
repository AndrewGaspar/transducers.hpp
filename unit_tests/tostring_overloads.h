#pragma once

#include <CppUnitTest.h>
#include <vector>
#include <list>
#include <set>

#include <transducers\interject.hpp>
#include <transducers\output.hpp>

template<typename T>
std::wstring collection_to_string(T const & t)
{
    using namespace transducers;

    std::wstringstream stream;
    output_to(interject(','), stream, t);
    return stream.str();
}

template<> static
std::wstring
Microsoft::VisualStudio::CppUnitTestFramework::ToString<std::vector<int>>(const std::vector<int>& t)
{
    return ::collection_to_string(t);
}

template<> static
std::wstring
Microsoft::VisualStudio::CppUnitTestFramework::ToString<std::vector<float>>(const std::vector<float>& t)
{
    return ::collection_to_string(t);
}

template<> static
std::wstring
Microsoft::VisualStudio::CppUnitTestFramework::ToString<std::list<int>>(const std::list<int>& t)
{
    return ::collection_to_string(t);
}

template<> static
std::wstring
Microsoft::VisualStudio::CppUnitTestFramework::ToString<std::set<int>>(const std::set<int>& t)
{
    return ::collection_to_string(t);
}