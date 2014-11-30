#pragma once

#include "tostring_overloads.h"

#include <transducers\mapping.hpp>

template<int x>
class Adds
{
public:
    template<typename T>
    constexpr T operator()(T input) const
    {
        return T(input + x);
    }
};

class Increments : public Adds<1>
{};

class Decrements : public Adds<-1>
{};

auto const incrementing = transducers::mapping(Increments());
auto const decrementing = transducers::mapping(Decrements());