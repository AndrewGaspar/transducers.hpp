#include <vector>
#include <chrono>
#include <iostream>
#include <functional>
#include <sstream>
#include <algorithm>
#include <numeric>

#include <transducers\transduce.hpp>
#include <transducers\mapping.hpp>
#include <transducers\filtering.hpp>
#include <transducers\compose.hpp>
#include <transducers\taking.hpp>
#include <transducers\skipping.hpp>
#include <transducers\interjecting.hpp>
#include <transducers\into_vector.hpp>
#include <transducers\output.hpp>
#include <transducers\output_to_string.hpp>
#include <transducers\reducer.hpp>

#include "range.hpp"
#include "repeats.hpp"

using namespace transducers;

class Timer
{
public:
    template<typename Fun>
    auto operator()(Fun const & f) const
    {
        auto start = std::chrono::high_resolution_clock::now();
        f();
        return std::chrono::high_resolution_clock::now() - start;
    }
};

template<typename dur>
class DurationCaster
{
public:
    template<typename From>
    dur operator()(From input) const
    {
        return std::chrono::duration_cast<dur>(input);
    }
};

class Hashes
{
public:
    template<typename T>
    size_t operator()(T input) const
    {
        return std::hash<T>()(input);
    }
};

template<size_t mod>
struct ModuloFilter
{
    template<typename T>
    constexpr bool operator()(T input) const
    {
        return !(input % mod);
    }
};

static const auto hashing = mapping(Hashes());
static const auto hashing_filtering = compose(hashing, filtering(ModuloFilter<128>()));

static const auto accumulater = reducer([](size_t acc, size_t x) { return acc + x; });

size_t func1()
{
    auto my_range = range<size_t>(0, size_t(1e8));
    return transduce(
        my_range.begin(), my_range.end(), 0,
        hashing_filtering,
        accumulater);
}

size_t func2()
{
    std::hash<size_t> size_t_hash;

    size_t acc = 0;
    for (auto const & x : range<size_t>(0, size_t(1e8)))
    {
        auto my_hash = size_t_hash(x);
        if (my_hash % size_t(128) == 0)
        {
            acc += my_hash;
        }
    }
    return acc;
}

size_t func3()
{
    std::hash<size_t> size_t_hash;

    auto my_range = range<size_t>(0, size_t(1e8));

    std::vector<size_t> hashes;
    std::transform(my_range.begin(), my_range.end(), std::back_inserter(hashes), [&size_t_hash](size_t input) {
        return size_t_hash(input);
    });

    auto end = std::remove_if(hashes.begin(), hashes.end(), [](size_t input) {
        return !(input % 128);
    });

    return std::accumulate(hashes.begin(), end, 0);
}

int main(int count, char** args)
{
    std::vector<std::string> arguments(args, args + count);
    
    using size_t_function = std::function<size_t(void)>;

    std::vector<size_t_function> funcs { func1, func2, func3 };

    output_to(
        compose(
            mapping([](std::vector<size_t_function> const & my_funcs) {
                return into_vector(my_funcs, compose(mapping(Timer()), mapping(DurationCaster<std::chrono::milliseconds>())));
            }),
            skipping(1),
            taking(10),
            mapping([](std::vector<std::chrono::milliseconds> const & my_durations) {
                return output_to_string(
                    compose(
                        mapping([](std::chrono::milliseconds ms) {
                            std::ostringstream stream;
                            stream << ms.count() << " ms";
                            return stream.str();
                        }),
                        interjecting(',')
                    ), 
                    my_durations);
            }),
            interjecting('\n')
        ),
    std::cout, repeats(&funcs, (&funcs)+1));

    std::wcin.get();
}