#pragma once

template<typename integer>
class RangeIterator
{
private:
    integer m_value;
public:
    explicit RangeIterator(integer value) :m_value(value) {}
    RangeIterator(RangeIterator const &) = default;
    RangeIterator(RangeIterator&&) = default;

    integer operator*() const
    {
        return m_value;
    }

    bool operator==(RangeIterator const & ri) const
    {
        return this->m_value == ri.m_value;
    }

    bool operator!=(RangeIterator const & ri) const
    {
        return this->m_value != ri.m_value;
    }

    RangeIterator & operator++()
    {
        ++m_value;
        return *this;
    }

    RangeIterator & operator--()
    {
        --m_value;
        return *this;
    }

    RangeIterator operator++(int)
    {
        RangeIterator copy(*this);
        ++(*this);
        return copy;
    }

    RangeIterator operator--(int)
    {
        RangeIterator copy(*this);
        --(*this);
        return copy;
    }
};

template<typename integer>
class RangeEnumerator
{
    integer m_start;
    integer m_stop;
public:
    RangeEnumerator(integer start, integer stop) : m_start(start), m_stop(stop) {}

    RangeIterator<integer> begin() const
    {
        return RangeIterator<integer>(m_start);
    }

    RangeIterator<integer> end() const
    {
        return RangeIterator<integer>(m_stop);
    }
};

template<typename integer>
auto range(integer start, integer stop)
{
    return RangeEnumerator<integer>(start, stop);
}