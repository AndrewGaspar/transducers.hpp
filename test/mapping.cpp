#include <algorithm>
#include <iostream>
#include <type_traits>
#include <memory>
#include <vector>
#include <sstream>
#include <cstdio>
#include <functional>

#include <experimental/generator>
#include <random>
#include <chrono>

#include "transducers/compose.hpp"
#include "transducers/mapping.hpp"
#include "transducers/filtering.hpp"
#include "transducers/transduce.hpp"
#include "transducers/into.hpp"
#include "transducers/output.hpp"
#include "transducers/interject.hpp"
#include "transducers/bookend.hpp"
#include "transducers/taking.hpp"

using namespace transducers;

template<typename num_type>
class Multiplier
{
    num_type m_multiplier;
public:
    Multiplier(num_type multiplier) : m_multiplier(multiplier) {}
    num_type operator()(num_type multiplicand) const
    {
        return m_multiplier * multiplicand;
    }
};

class AlwaysTrue
{
public:
    template<typename T>
    bool operator()(T&&)
    {
        return true;
    }
};

auto random_engine = std::default_random_engine(unsigned int(std::chrono::system_clock::now().time_since_epoch().count()));

template<typename _Integer>
std::experimental::generator<_Integer> random_uniform_values(_Integer lower, _Integer upper)
{
    std::uniform_int_distribution<_Integer> distribution(lower, upper);
    for (;;)
    {
        __yield_value distribution(random_engine);
    }
}

char maps_byte_to_char(uint8_t x)
{
    return (x < 26) ? 'a' + x : 'A' + (x - 26);
}

uint8_t maps_char_to_byte(char c)
{
    return c > 'A' ? (c - 'A') + 26 : c - 'a';
}

auto byte_to_char = mapping(maps_byte_to_char);
auto newline = bookend('\n');
auto comma_separate = interject(',');
auto newline_separate = interject('\n');
auto list_formatting = compose(comma_separate,newline);

std::experimental::generator<std::string> random_strings(size_t lowerSize, size_t upperSize)
{
    using namespace transducers;

    std::uniform_int_distribution<size_t> distribution(lowerSize, upperSize);

    for (;;)
    {
        std::stringstream s_stream;
        output_to(compose(byte_to_char, taking(distribution(random_engine))), s_stream, random_uniform_values(0, 51));
        __yield_value s_stream.str();
    }
}

template<typename _Integer>
std::experimental::generator<_Integer> range(_Integer from, _Integer to)
{
    for (; from < to; from++)
    {
        __yield_value from;
    }
}

void from_count(std::stringstream & s_stream, size_t count)
{
    if (count >= 52)
    {
        from_count(s_stream, (count / 52)-1);
    }
    s_stream << maps_byte_to_char(count % 52);
}

std::experimental::generator<std::string> counting_strings()
{

    for (auto & x : range<size_t>(0, std::numeric_limits<size_t>::max()))
    {
        std::stringstream myStream;
        from_count(myStream, x);
        __yield_value myStream.str();
    }
}

auto get_cool_transducer()
{
    using namespace transducers;

    return compose(
        filtering([](int x) { return x != 0; }),
        mapping(Multiplier<int>(2)),
        mapping([](int x) { return x - 7; }),
        filtering([](int x) { return x < 0; }),
        mapping([](int x) -> float { return float(x); }),
        mapping(Multiplier<float>(1.0f / 7.0f)),
        taking(2)
    );
}

std::string capitalize(std::string const & string)
{
    std::stringstream str_stream;
    output_to(mapping([](char a) { return char(::towupper(a)); }), str_stream, string);
    return str_stream.str();
}

auto capitalizes = mapping(capitalize);

auto reverses = mapping([](std::string const & string) {
    return std::string(std::make_reverse_iterator(string.end()), std::make_reverse_iterator(string.begin()));
});

template<typename T>
size_t hash(T const & thing)
{
    std::hash<typename std::remove_reference<T>::type> hash_fun;
    return hash_fun(thing);
}

struct Hasher
{
    template<typename T>
    size_t operator()(T&& thing) const
    {
        return hash(std::forward<T>(thing));
    }
};

struct HashPairer
{
    template<typename T>
    std::pair<typename std::remove_reference<T>::type, size_t> operator()(T&& thing) const
    {
        return{ std::forward<T>(thing), hash(thing) };
    }
};

struct HyphenSeparatePairs
{
    template<typename _PairThing>
    std::string operator()(_PairThing&& thing) const
    {
        std::stringstream s_stream;
        s_stream << thing.first << "\t-\t" << thing.second;
        return s_stream.str();
    }
};

template<typename T>
bool is_palindrone(T const & str)
{
    auto begin = str.begin();
    auto end = str.end();
    end--;

    while (begin < end)
    {
        if (*begin != *end) return false;

        begin++;
        end--;
    }

    return true;
}

auto newlines = compose(newline_separate, newline);

auto hashes = mapping(Hasher());
auto pair_with_hash = mapping(HashPairer());
auto hyphen_separate = mapping(HyphenSeparatePairs());
auto hash_and_hyphenate = compose(pair_with_hash, hyphen_separate);

class FuncTimer
{
public:
    template<typename Func>
    std::chrono::milliseconds operator()(Func const & f) const
    {
        auto start = std::chrono::system_clock::now();
        f();
        return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - start);
    }
};

int main()
{
    std::string password;

    do
    {
        std::cin >> password;
    } while (into_vector(compose(filtering([](char c) { return !::isalpha(c); }), taking(1)), password).size());

    auto pass_hash = hash(password);

    using void_function = std::function<void(void)>;

    void_function func1 = [&, pass_hash]() {
        auto guessing = compose(hashes, filtering([=](size_t const & hash) {return hash == pass_hash; }), taking(1));
        output_to(compose(guessing, newlines), std::cout, counting_strings());
    };

    void_function func2 = [&, pass_hash]() {
        for (auto const & guess : counting_strings())
        {
            auto hash_guess = hash(guess);
            if (hash_guess == pass_hash)
            {
                std::cout << hash_guess << std::endl;
                return;
            }
        }
    };

    auto prints_ms = [](std::chrono::milliseconds const & time) {
        std::stringstream stream;
        stream << "Elapsed time: " << time.count() << " ms";
        return stream.str();
    };

    auto format_times = compose(mapping(prints_ms), newlines);

    for (;;)
    {
        auto timings = into_vector(mapping(FuncTimer()), std::vector<void_function> { func1, func2 });

        output_to(format_times, std::cout, timings);
    }
}