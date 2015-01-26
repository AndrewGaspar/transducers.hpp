#include <vector>
#include <chrono>
#include <iostream>
#include <functional>
#include <sstream>
#include <algorithm>
#include <numeric>
#include <thread>
#include <random>

#include <transducers\transduce.hpp>
#include <transducers\mapping.hpp>
#include <transducers\multitransducing.hpp>
#include <transducers\filtering.hpp>
#include <transducers\compose.hpp>
#include <transducers\taking.hpp>
#include <transducers\skipping.hpp>
#include <transducers\interleaving.hpp>
#include <transducers\into_vector.hpp>
#include <transducers\into.hpp>
#include <transducers\output.hpp>
#include <transducers\output_to_string.hpp>
#include <transducers\reducer.hpp>
#include <transducers\repeating.hpp>
#include <transducers\sorting.hpp>
#include <transducers\inserting_after_each.hpp>
#include <transducers\beginning.hpp>
#include <transducers\mapcatting.hpp>

#include "range.hpp"
#include "repeats.hpp"
#include "perf_test.hpp"

using namespace transducers;

static const auto hashing = mapping(std::hash<uint32_t>());
static const auto accumulater = reducer([](size_t acc, size_t x) { return acc + x; });

static auto const addingMdTableBorders = compose(inserting_after_each(" | "), beginning("| "));
static auto const addingNewLines = inserting_after_each('\n');

static auto const printingTestInfo = compose(
    multitransducing(
        mapping([](perf_test const & perfTest) {
            std::stringstream stream;
            stream << "## " << perfTest.title();
            return stream.str();
        }),
        mapping([](perf_test const & perfTest) -> std::string const & {
            return perfTest.description();
        }),
        mapping([](perf_test const &) {
            return "### Results";
        })
    ),
    addingNewLines
);

static auto const printingResultsHeader = compose(
    multitransducing(
        mapping([](perf_test const & perfTest) {
            return output_to_string(
                compose(
                    mapping([](auto const & testFn) -> std::string const & {
                        return testFn->title();
                    }),
                    addingMdTableBorders
                ),
                perfTest.tests()
            );
        }),
        mapping([](perf_test const & perfTest) {
            return output_to_string(
                compose(
                    mapping([](auto const & testFn) {
                        std::string dashes(testFn->title().size()-1, '-');
                        dashes.append(":");
                        return dashes;
                    }),
                    addingMdTableBorders
                ),
                perfTest.tests()
            );
        })
    ),
    addingNewLines
);

static auto const computingAndPrintingTestResults = compose(
    repeating(10),
    multitransducing(
        mapping([](auto const & perfTest) { return "| "; }),
        compose(
            mapcatting([](perf_test const & perfTest) -> decltype(auto) {
                return perfTest.tests();
            }),
            mapping([](std::unique_ptr<virtual_test_function> const & testFunction) {
                auto time = testFunction->time_function();
                auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(time);

                std::ostringstream stream;
                stream << ms.count() << " ms";
                auto str = stream.str();
                auto const & title = testFunction->title();

                if (str.size() >= title.size())
                {
                    return str;
                }

                auto spaces = title.size() - str.size();

                std::string result(spaces, ' ');
                result += str;

                return result;
            }),
            inserting_after_each(" | ")
        ),
        mapping([](auto const &) { return '\n'; })
    )
);

int main(int count, char** args)
{
    std::vector<std::string> arguments(args, args + count);

    a_perf_test<size_t> hashing_test{
        "HashingFiltering",
        "Each test iterates through a lazily evaluated range of numbers, hashes them, filters out those "
        "divisible by 128, and sums them.",
        {
            {"transducers", []() {
                auto my_range = range<uint32_t>(0, 100000000);
                return transduce(
                    my_range.begin(), my_range.end(), 0,
                    compose(
                        hashing, 
                        filtering([](size_t x) { return !(x % 128); })
                    ),
                    accumulater);
            }},
            {"STL <algorithms>", []() {
                auto my_range = range<uint32_t>(0, 100000000);
                std::hash<uint32_t> hash;

                std::vector<size_t> hashes;
                hashes.reserve(100000000);
                std::transform(my_range.begin(), my_range.end(), std::back_inserter(hashes), hash);

                auto end = std::remove_if(hashes.begin(), hashes.end(), [](size_t x) { return x % 128; });

                return std::accumulate(hashes.begin(), end, size_t(0));
            }},
            {"for-loop", []() {
                std::hash<uint32_t> hash;

                size_t acc = 0;
                for (auto const & x : range<uint32_t>(0, 100000000))
                {
                    auto my_hash = hash(x);
                    if (my_hash % 128 == 0)
                    {
                        acc += my_hash;
                    }
                }

                return acc;
            }}
        }
    };

    std::seed_seq seq{ 9222,928188,12122993,72,6 };
    std::vector<std::uint32_t> seeds(1000000);
    seq.generate(seeds.begin(), seeds.end());

    a_perf_test<std::vector<std::uint32_t>> sorting_test{
        "Sorting Pseudo-random Numbers",
        "This performance test sorts 1-million random numbers and returns the first thousand.",
        {
            {"transducers", [&seeds]() {
                // allegedly produces deterministic series of numbers uniformly distributed from 0 to 2^32-1
                return into_vector(seeds, compose(sorting<std::uint32_t>(), taking(1000)));
            }},
            {"std::sort (sorts all)", [&seeds]() {
                auto my_seeds = seeds;

                std::sort(my_seeds.begin(), my_seeds.end());
                return std::vector<std::uint32_t>(my_seeds.begin(), my_seeds.begin() + 1000);
            }},
            {"std::make_heap (sorts first 1000)", [&seeds]() {
                auto my_seeds = seeds;

                auto begin = my_seeds.rbegin();
                auto end = my_seeds.rend();

                auto pr = std::greater<std::uint32_t>{};

                std::make_heap(begin, end, pr);
                for (auto i : range(0, 1000))
                {
                    std::pop_heap(begin, end--, pr);
                }
                return std::vector<std::uint32_t>(my_seeds.begin(), my_seeds.begin() + 1000);
            }}
        }
    };

    std::vector<perf_test> perf_tests;
    perf_tests.emplace_back(std::move(hashing_test));
    perf_tests.emplace_back(std::move(sorting_test));
    
    using size_t_function = std::function<size_t(void)>;


    output_to(
        multitransducing(
            printingTestInfo,
            printingResultsHeader,
            computingAndPrintingTestResults
        ),
    std::cout, perf_tests);

    //std::cin.get();
}