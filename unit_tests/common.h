#pragma once

#include "tostring_overloads.h"

#include <transducers\mapping.hpp>

class Increments
{
public:
    template<typename T>
    T operator()(T input) const
    {
        return input + 1;
    }
};

auto incrementing = transducers::mapping(Increments());