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
#include <transducers\into.hpp>
#include <transducers\output.hpp>
#include <transducers\output_to_string.hpp>
#include <transducers\reducer.hpp>

#include "range.hpp"
#include "repeats.hpp"
#include "perf_test.hpp"

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

static const auto hashing = mapping(std::hash<uint32_t>());
static const auto accumulater = reducer([](size_t acc, size_t x) { return acc + x; });

int main(int count, char** args)
{
    std::vector<std::string> arguments(args, args + count);

    a_perf_test<size_t> hashing_tests{
        "HashingFiltering",
        "Each test iterates through a lazily evaluated range of numbers, hashes them, filters out those"
        "divisible by 128, and sums them.",
        {
            {"transducers", []() {
                auto my_range = range<uint32_t>(0, 1000000000);
                return transduce(
                    my_range.begin(), my_range.end(), 0,
                    compose(
                        hashing, 
                        filtering([](size_t x) { return !(x % 128); })
                    ),
                    accumulater);
            }},
            {"for-loop", []() {
                std::hash<uint32_t> size_t_hash;

                size_t acc = 0;
                for (auto const & x : range<uint32_t>(0, 1000000000))
                {
                    auto my_hash = size_t_hash(x);
                    if (my_hash % 128 == 0)
                    {
                        acc += my_hash;
                    }
                }

                return acc;
            }}
        }
    };
    
    using size_t_function = std::function<size_t(void)>;

    output_to(
        compose(
            mapping([](a_perf_test<size_t> const & perfTest) {
                return into_vector(perfTest.tests(),
                    compose(
                        mapping([](std::unique_ptr<virtual_test_function> const & testFunction) {
                            return testFunction->time_function();
                        }),
                        mapping(DurationCaster<std::chrono::milliseconds>{})
                    ));
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
    std::cout, repeats(&hashing_tests, (&hashing_tests)+1));

    std::wcin.get();
}