#pragma once

template<typename _InIt>
class RepeatIterator
{
    bool m_is_end = false;
    _InIt m_begin, m_me, m_end;
public:
    RepeatIterator(_InIt begin, _InIt end) : m_begin(begin), m_me(begin), m_end(end) {}
    RepeatIterator() : m_is_end(true) {}

    bool is_end() const
    {
        return m_is_end;
    }

    auto & operator*() const
    {
        return *m_me;
    }

    RepeatIterator & operator++()
    {
        ++m_me;
        if (m_me == m_end) { m_me = m_begin; }
        return (*this);
    }

    RepeatIterator operator++(int)
    {
        RepeatIterator copy(*this);
        ++(*this);
        return copy;
    }

    bool operator==(RepeatIterator const & other)
    {
        if (is_end() || other.is_end()) { return is_end() == other.is_end(); }

        return m_me == other.m_me;
    }

    bool operator!=(RepeatIterator const & other)
    {
        if (is_end() || other.is_end()) { return is_end() != other.is_end(); }

        return m_me != other.m_me;
    }
};

template<typename _InIt>
class RepeatEnumerator
{
    _InIt m_begin, m_end;
public:
    RepeatEnumerator(_InIt begin, _InIt end) : m_begin(begin), m_end(end) {}

    RepeatIterator<_InIt> begin() const
    {
        return RepeatIterator<_InIt>(m_begin, m_end);
    }

    RepeatIterator<_InIt> end() const
    {
        return RepeatIterator<_InIt>();
    }
};

template<typename _InIt>
auto repeats(_InIt begin, _InIt end)
{
    return RepeatEnumerator<_InIt>(begin, end);
}