#include <vector>
#include <chrono>
#include <iostream>
#include <functional>
#include <sstream>
#include <algorithm>
#include <numeric>

#include <transducers\transduce.hpp>
#include <transducers\mapping.hpp>
#include <transducers\multitransducing.hpp>
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

template<typename duration>
auto duration_casting()
{
    return mapping([](auto const & from) {
        return std::chrono::duration_cast<duration>(from);
    });
}

static const auto hashing = mapping(std::hash<uint32_t>());
static const auto accumulater = reducer([](size_t acc, size_t x) { return acc + x; });

int main(int count, char** args)
{
    std::vector<std::string> arguments(args, args + count);

    a_perf_test<size_t> hashing_test{
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

    std::vector<perf_test> perf_tests;
    perf_tests.emplace_back(std::move(hashing_test));
    
    using size_t_function = std::function<size_t(void)>;

    auto printingTest = multitransducing(
        mapping([](perf_test const & perfTest) {
            std::stringstream stream;
            stream << "Title: " << perfTest.title() << '\n';
            return stream.str();
        }),
        mapping([](perf_test const & perfTest) {
            std::stringstream stream;
            stream << "Description: " << perfTest.description() << '\n';
            return stream.str();
        })
    );

    output_to(
        /*multitransducing(
            printingTest,*/
            compose(
                mapping([](perf_test const & perfTest) {
                    return into_vector(perfTest.tests(),
                        compose(
                            mapping([](auto const & testFunction) {
                                return testFunction->time_function();
                            }),
                            duration_casting<std::chrono::milliseconds>()
                        ));
                }),
                skipping(1),
                taking(10),
                mapping([](auto const & my_durations) {
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
            )
        //)
        ,
    std::cout, repeats(perf_tests.cbegin(), perf_tests.cend()));

    std::wcin.get();
}