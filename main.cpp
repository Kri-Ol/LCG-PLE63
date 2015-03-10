#include <cassert>
#include <cstdint>

#include <cmath>

#include <algorithm>
#include <utility>

#include "LCG_PLE63.hpp"
#include "std_LCG_PLE63.hpp"
#include "uniform_distribution.hpp"

static uint64_t find_period(uint64_t N, std::ostream& os)
{
    constexpr uint32_t BILLION = 1024U*1024U*1024U;

    std::linear_congruential_engine<uint64_t, 2806196910506780709ULL, 1ULL, (1ULL<<63ULL)> ugen;
    auto seed = ugen.get_seed();

    N /= BILLION; // number of billions

    for( auto i = 0ULL; i != N; ++i )
    {
        for( auto k = 0U; k != BILLION; ++k)
        {
            auto s = ugen();
            if (s == seed)
            {
                return i*BILLION + uint64_t(k);
            }
        }
        os << "Passed: " << i << std::endl;
    }
    // skip the leftover

    return 0ULL;
}

static inline std::pair<float,float> kahan_summation(std::pair<float,float> s, float r)
{
    float y = r - s.second;
    float t = s.first + y;

    return std::pair<float,float>{t, (t - s.first) - y};
}

static std::pair<float,float> test_mean_sigma(uint64_t n)
{
    std::pair<float,float> s{0.0f, 0.0f};
    std::pair<float,float> d{0.0f, 0.0f};

    std::linear_congruential_engine<uint64_t, 2806196910506780709ULL, 1ULL, (1ULL<<63ULL)> ugen;
    std::uniform_distribution<float> rng;

    for(uint64_t k = 0ULL; k != n; ++k)
    {
        float r = rng(ugen);
        
        s = kahan_summation(s, r);
        d = kahan_summation(d, r*r);
    }

    s.first /= float(n);
    d.first /= float(n);

    d.first = sqrt(d.first);
    d.first = (d.first - s.first)*(d.first + s.first);
    if (d.first < 0.0f)
        d.first = 0.0f;

    return std::pair<float,float>{s.first, d.first};
}

static bool test_skip_ahead(int64_t ns)
{
    assert( ns > 0LL);

    std::linear_congruential_engine<uint64_t, 2806196910506780709ULL, 1ULL, (1ULL<<63ULL)> ugen;
    std::linear_congruential_engine<uint64_t, 2806196910506780709ULL, 1ULL, (1ULL<<63ULL)> ugens{ugen};

    ugens.discard(ns);

    // manual skip ahead
    uint64_t sum{0ULL};
    for(int64_t k = 0; k != ns; ++k)
    {
        auto r = ugen();
        sum += r;
    }

    // compare manual and fast skip
    return (ugen.get_seed() == ugens.get_seed());
}

static bool test_skip_ahead_and_back(int64_t ns)
{
    assert( ns > 0LL);

    std::linear_congruential_engine<uint64_t, 2806196910506780709ULL, 1ULL, (1ULL<<63ULL)> ugen;
    auto seed = ugen.get_seed();

    ugen.discard(ns);
    ugen.discard(-ns);

    return (ugen.get_seed() == seed);
}

static bool test_skip_zero()
{
    std::linear_congruential_engine<uint64_t, 2806196910506780709ULL, 1ULL, (1ULL<<63ULL)> ugen;

    auto seed = ugen.get_seed();

    ugen.discard(0LL);

    return (ugen.get_seed() == seed);
}

static bool test_skip_backward(int64_t ns)
{
    assert( ns < 0LL);

    std::linear_congruential_engine<uint64_t, 2806196910506780709ULL, 1ULL, (1ULL<<63ULL)> ugen;
    auto seed = ugen.get_seed();

    // internal skip-ahead
    ugen.discard(ns);

    // manual skip-ahead by same number of steps
    uint64_t fns = abs(ns);
    uint64_t sum{0ULL};
    for(uint64_t k = 0ULL; k != fns; ++k)
    {
        auto q = ugen();
        sum += q;
    }

    return (ugen.get_seed() == seed);
}

static bool test_skip_backward_and_back(int64_t ns)
{
    assert( ns < 0LL);

    std::linear_congruential_engine<uint64_t, 2806196910506780709ULL, 1ULL, (1ULL<<63ULL)> ugen;
    auto seed = ugen.get_seed();

    ugen.discard(ns);
    ugen.discard(-ns);

    return (ugen.get_seed() == seed);
}

static bool test_custom_vs_std(uint64_t ns)
{
    OTI::lcg_PLE63 rng;
    std::linear_congruential_engine<uint64_t, 2806196910506780709ULL, 1ULL, (1ULL<<63ULL)> rng_std;

    for(uint64_t k = 0; k != ns; ++k)
    {
        rng.sample();
        auto rseed = rng.seed();

        auto sseed = rng_std();

        if (rseed != sseed)
            return false;
    }

    return true;
}

static bool test_skip_custom_vs_std(int64_t ns)
{
    OTI::lcg_PLE63 rng;
    std::linear_congruential_engine<uint64_t, 2806196910506780709ULL, 1ULL, (1ULL<<63ULL)> rng_std;

    rng.skip(ns);
    OTI::lcg_PLE63::seed_type rseed = rng.seed();

    rng_std.discard(ns);
    OTI::lcg_PLE63::seed_type sseed = rng_std.get_seed();

    return (rseed == sseed);
}

int main(int argc, char* argv[])
{
    OTI::lcg_PLE63::result_type period = find_period(2500000000ULL, std::cout); 
    std::cout << "Found period: " << period << std::endl;

    std::pair<float,float> r = test_mean_sigma(50000000ULL);
    std::cout << "Mean, Sigma " << r.first*2.0f << " " << r.second*12.0f << std::endl;

    bool q;

    q = test_skip_ahead(777777LL);
    assert(q);

    q = test_skip_ahead_and_back(12391LL);
    assert(q);
    
    test_skip_zero();
    assert(q);

    q = test_skip_backward(-7788991LL);
    assert(q);

    q = test_skip_backward_and_back(-12391LL);
    assert(q);

    q = test_custom_vs_std(10ULL);
    assert(q);

    q = test_skip_custom_vs_std(1336789ULL);
    assert(q);

    q = test_skip_custom_vs_std(-123987LL);
    assert(q);

    return 0;
}

