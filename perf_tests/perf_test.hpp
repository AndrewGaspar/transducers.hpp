#pragma once

#include <utility>
#include <string>
#include <vector>
#include <functional>
#include <chrono>
#include <memory>

#include <transducers\into.hpp>
#include <transducers\mapping.hpp>

class virtual_test_function
{
    std::string m_title;
public:
    virtual_test_function(std::string title) : m_title(std::move(title)) {}
    virtual ~virtual_test_function() = default;

    std::string const & title() const
    {
        return m_title;
    }

    virtual std::chrono::high_resolution_clock::duration time_function() const = 0;
};

template<typename _Out>
class test_function : public virtual_test_function
{
    std::function<_Out(void)> m_test;
public:
    test_function(std::string title, std::function<_Out(void)> test) : 
        virtual_test_function(std::move(title)),
        m_test(std::move(test)) 
    {
    }

    virtual std::chrono::high_resolution_clock::duration time_function() const
    {
        auto start = std::chrono::high_resolution_clock::now();
        auto result = m_test();
        return std::chrono::high_resolution_clock::now() - start;
    }
};

class perf_test
{
    std::string m_title;
    std::string m_description;
protected:
    std::vector<std::unique_ptr<virtual_test_function>> m_tests;
public:
    perf_test(std::string title, std::string description) : 
        m_title(std::move(title)), 
        m_description(std::move(description)) 
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

    std::vector<std::unique_ptr<virtual_test_function>> const & tests() const
    {
        return m_tests;
    }
};

template<typename _Out>
class a_perf_test : public perf_test
{
public:
    a_perf_test(std::string title, std::string description,
        std::initializer_list<test_function<_Out>> tests) :
        perf_test(std::move(title), std::move(description))
    {
        for (auto & test : tests)
        {
            m_tests.push_back(std::unique_ptr<test_function<_Out>>(new test_function<_Out>{ test }));
        }
    }
};