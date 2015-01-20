#pragma once

#include <random>

template<typename result_type>
class RandomEnumerator
{
    std::uniform_int_distribution<result_type> dist;
public:

};

template<typename value_type>
class RandomIterator
{
    std::default_random_engine::result_type m_seed;
    std::uniform_int_distribution<value_type> m_dist;
public:
    RandomIterator(value_type min, value_type max, std::default_random_engine::result_type seed) : m_min(min), m_max(max), m_seed(seed) {}

    value_type min() const { return m_dist.min(); }
    value_type max() const { return m_dist.max(); }

    RandomEnumerator<value_type> begin() const
    {
        std::default_random_engine en{ m_seed };
    }

    RandomEnumerator<value_type> end() const
    {

    }
};

template<typename value_type>
auto random_values(value_type min, value_type max, std::default_random_engine::result_type seed)
{
}