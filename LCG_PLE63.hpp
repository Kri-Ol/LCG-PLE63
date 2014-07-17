// -*- C++ -*-

#pragma once

#include <cstdint>
#include <utility>

namespace OTI
{
    class lcg_PLE63
    {
#pragma region Typedefs/Constants
        public: typedef uint_fast64_t result_type;
        public: typedef result_type   seed_type;

        public: static constexpr result_type  nbits = 63ULL;
        public: static constexpr result_type  mod   = 1ULL << nbits;
        public: static constexpr result_type  mask  = mod - 1ULL;

        // those values are from P.L'Ecuyer "Efficient and Portable Combined RNGs", Comm.ACM 31(6): 742-749, 774 (1988)
        public: static constexpr result_type  mult  = 2806196910506780709ULL;
        public: static constexpr result_type  add   = 1ULL;

        public: static constexpr seed_type  default_seed   = 1ULL;        

        public: static constexpr float        norm  = float{1.0842021724855044340074528008699e-19};
#pragma endregion

#pragma region Data
        private: mutable seed_type _seed;
#pragma endregion

#pragma region Ctor/Dtor/op=
        public: lcg_PLE63(seed_type seed = default_seed):
            _seed{seed}
        {
        }

        public: lcg_PLE63(lcg_PLE63 const& r):
            _seed{r._seed}
        {
        }

        public: lcg_PLE63(lcg_PLE63&& r):
            _seed{std::move(r._seed)}
        {
        }        

        public: lcg_PLE63& operator=(lcg_PLE63 const& r)
        {
            _seed = r._seed;
            return *this;
        }

        public: lcg_PLE63& operator=(lcg_PLE63&& r)
        {
            _seed = std::move(r._seed);
            return *this;
        }

        public: ~lcg_PLE63()
        {
        }
#pragma endregion

#pragma region Observers
        public: seed_type seed() const
        {
            return _seed;
        }
#pragma endregion

#pragma region Helpers

        // compute and return positive skip
        public: static int64_t compute_nskip(int64_t ns);

        // could skip forward as well as backward
        public: static result_type skip(int64_t ns, seed_type seed);

        public: static result_type sample(seed_type seed)
        {
            return (mult * seed + add) & mask;
        }
#pragma endregion

#pragma region Mutators
        public: void sample() const
        {
            _seed = sample(_seed);
        }

        public: float number() const
        {
            sample();
            return float(_seed) * norm;
        }

        public: void skip(int64_t ns) const
        {
            _seed = skip(ns, _seed);
        }
#pragma endregion
    };
}
