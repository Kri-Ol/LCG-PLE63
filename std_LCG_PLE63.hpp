// -*- C++ -*-

#pragma once

#include <cstdint>
#include <random>

namespace std
{
    template <uint_fast64_t A, uint_fast64_t C, uint_fast64_t M> class linear_congruential_engine<uint_fast64_t, A, C, M>
    {
#pragma region Typedefs/Constants
        public: typedef uint_fast64_t result_type;
        public: typedef result_type   seed_type;

        public: static constexpr result_type multiplier = A;
        public: static constexpr result_type increment  = C;
        public: static constexpr result_type modulus    = M;

        public: static constexpr result_type mask = modulus - 1ULL;

        public: static constexpr seed_type default_seed = 1ULL;
#pragma region Data
        private: seed_type _seed;
#pragma endregion

#pragma region Ctor/Dtor/op=
        public: explicit linear_congruential_engine(seed_type seed = default_seed):
            _seed(seed)
        {
        }

        public: linear_congruential_engine(const linear_congruential_engine& r):
            _seed(r._seed)
        {
        }

        public: linear_congruential_engine& operator=(const linear_congruential_engine& r)
        {
            _seed = r._seed;
            return *this;
        }

        public: ~linear_congruential_engine()
        {
        }
#pragma endregion

#pragma region Observers
        public: void seed(seed_type seed = default_seed)
        {
            _seed = seed;
        }

        public: seed_type the_seed() const
        {
            return _seed;
        }
#pragma endregion

#pragma region Helpers
        public: static result_type min()
        {
            return 0ULL;
        }

        public: static result_type max()
        {
            return mask;
        }

        // could skip forward as well as backward
        public: static result_type skip(result_type ns, result_type seed)
        {
            int64_t nskip = ns & mask;

            // The algorithm here to determine the parameters used to skip ahead is
            // described in the paper F. Brown, "Random Number Generation with Arbitrary Stride,"
            // Trans. Am. Nucl. Soc. (Nov. 1994). This algorithm is able to skip ahead in
            // O(log2(N)) operations instead of O(N). It computes parameters
            // M and C which can then be used to find x_N = M*x_0 + C mod 2^M.

            // initialize constants
            result_type m = multiplier; // original multiplicative constant
            result_type c = increment;  // original additive constant

            result_type m_next = 1ULL; // new effective multiplicative constant
            result_type c_next = 0ULL; // new effective additive constant

            while (nskip > 0LL)
            {
                if (nskip & 1LL) // check least significant bit for being 1
                {
                    m_next = (m_next * m) & mask;
                    c_next = (c_next * m + c) & mask;
                }

                c = ((m + 1ULL) * c) & mask;
                m = (m * m) & mask;

                nskip = nskip >> 1LL; // shift right, dropping least significant bit
            }

            // with M and C, we can now find the new seed
            return (m_next * seed + c_next) & mask;
        }
#pragma endregion

#pragma region Mutators
        public: result_type operator()()
        {
            _seed = (multiplier * _seed + increment) & mask;
            return _seed;
        }

        public: void discard(result_type ns)
        {
            _seed = skip(ns, _seed);
        }
#pragma endregion
    };

    template <uint_fast64_t, uint_fast64_t A, uint_fast64_t C, uint_fast64_t M> bool operator==(
		const linear_congruential_engine<uint_fast64_t, A, C, M>& left,
		const linear_congruential_engine<uint_fast64_t, A, C, M>& rght)
	{
        return (left.the_seed() == rght.the_seed());
    }

    template <uint_fast64_t, uint_fast64_t A, uint_fast64_t C, uint_fast64_t M> bool operator!=(
		const linear_congruential_engine<uint_fast64_t, A, C, M>& left,
		const linear_congruential_engine<uint_fast64_t, A, C, M>& rght)
	{
        return (left.the_seed() != rght.the_seed());
    }

    template<typename CharT, typename Traits,
             uint_fast64_t,
             uint_fast64_t A, uint_fast64_t C, uint_fast64_t M>
	basic_istream<CharT, Traits>& operator>>( basic_istream<CharT, Traits>& is, linear_congruential_engine<uint_fast64_t, A, C, M>& eng)
    {
        uint_fast64_t seed;
        is >> seed;
        eng.seed(seed);
        return is;
    }

    template<typename CharT, typename Traits,
             uint_fast64_t,
             uint_fast64_t A, uint_fast64_t C, uint_fast64_t M>
	basic_ostream<CharT, Traits>& operator<<( basic_ostream<CharT, Traits>& os, const linear_congruential_engine<uint_fast64_t, A, C, M>& eng)
    {
        os << eng.the_seed();
        return os;
    }
}
