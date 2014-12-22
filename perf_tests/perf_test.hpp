#pragma once

#include <utility>
#include <string>
#include <vector>
#include <functional>
#include <chrono>

#include <transducers\into.hpp>
#include <transducers\mapping.hpp>

template<typename _Out>
class test_function
{
    std::string m_title;
    std::function<_Out(void)> m_test;
public:
    test_function(std::string title, std::function<_Out(void)> test) : 
        m_title(std::move(title)), 
        m_test(std::move(test)) 
    {
    }

    std::pair<_Out, std::chrono::milliseconds> execute_and_time() const
    {
        auto start = std::chrono::high_resolution_clock::now();
        auto result = m_test();
        auto time = std::chrono::high_resolution_clock::now() - start;
        return { result, std::chrono::duration_cast<std::chrono::milliseconds>(time) };
    }

    std::string const & title() const
    {
        return m_title;
    }
};

template<typename _Out>
class perf_test
{
    std::string m_title;
    std::string m_description;
    std::vector<test_function<_Out>> m_tests;
public:
    perf_test(std::string title, std::string description) : 
        m_title(std::move(title)), 
        m_description(std::move(description)) 
    {
    }

    perf_test(std::string title, std::string description,
        std::initializer_list<test_function<_Out>> tests) :
        m_title(std::move(title)),
        m_description(std::move(description)),
        m_tests(tests)
    {
    }

    std::string const & title() const
    {
        return m_title;
    }

    std::string const & description() const
    {
        return m_description;
    }

    std::vector<test_function<_Out>> const & tests() const
    {
        return m_tests;
    }

    void add_test(std::string title, std::function<_Out(void)> test)
    {
        m_tests.emplace_back(std::move(title), std::move(test));
    }

    void add_test(test_function<_Out> const & test)
    {
        m_tests.emplace_back(test);
    }

    void add_tests(std::initializer_list<test_function<_Out>> const & tests)
    {
        m_tests.insert(m_tests.back(), tests);
    }
};