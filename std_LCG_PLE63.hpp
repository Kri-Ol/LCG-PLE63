// -*- C++ -*-

#pragma once

#include <cstdint>
#include <random>
#include <iostream>

namespace std
{
    template <uint64_t A, uint64_t C, uint64_t M> class linear_congruential_engine<uint64_t, A, C, M>
    {
#pragma region Typedefs/Constants
        public: using result_type = uint64_t;

        public: static constexpr result_type multiplier   = A;
        public: static constexpr result_type increment    = C;
        public: static constexpr result_type modulus      = M;

        public: static constexpr result_type mask         = M - 1ULL;

        public: static constexpr result_type default_seed = 1ULL;
#pragma endregion

#pragma region Data
        private: result_type _seed;
#pragma endregion

#pragma region Ctor/Dtor/op=
        public: explicit linear_congruential_engine(result_type seed = default_seed):
            _seed{seed}
        {
        }
        
        public: linear_congruential_engine(const linear_congruential_engine& r) = default;
        public: linear_congruential_engine(linear_congruential_engine&& r)      = default;

        public: linear_congruential_engine& operator=(const linear_congruential_engine& r) = default;
        public: linear_congruential_engine& operator=(linear_congruential_engine&& r)      = default;

        public: ~linear_congruential_engine()
        {
        }
#pragma endregion

#pragma region Observers
        public: result_type get_seed() const
        {
            return _seed;
        }
#pragma endregion

#pragma region Helpers
        public: static constexpr result_type min()
        {
            return result_type{C == 0u};
        }

        public: static constexpr result_type max()
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
        public: void seed(result_type s = default_seed)
        {
            _seed = s;
        }

        public: result_type operator()()
        {
            return (_seed = (multiplier * _seed + increment) & mask);
        }

        public: void discard(unsigned long long ns)
        {
            _seed = skip(ns, _seed);
        }
#pragma endregion
    };

    template <uint64_t, uint64_t A, uint64_t C, uint64_t M> bool operator==(const linear_congruential_engine<uint64_t, A, C, M>& left,
                          		                                            const linear_congruential_engine<uint64_t, A, C, M>& rght)
    {
        return (left.get_seed() == rght.get_seed());
    }

    template <uint64_t, uint64_t A, uint64_t C, uint64_t M> bool operator!=(const linear_congruential_engine<uint64_t, A, C, M>& left,
 		                                                                    const linear_congruential_engine<uint64_t, A, C, M>& rght)
    {
        return (left.get_seed() != rght.get_seed());
    }

    template<typename CharT, typename Traits, uint64_t, uint64_t A, uint64_t C, uint64_t M>
	basic_istream<CharT, Traits>& operator>>( basic_istream<CharT, Traits>& is, linear_congruential_engine<uint64_t, A, C, M>& eng)
    {
        const auto sflags = is.flags();
        is.flags(std::basic_istream<CharT, Traits>::ios_base::dec);

        uint64_t seed;
        is >> seed;
        eng.seed(seed);
        
        is.flags(sflags);
        return is;
    }

    template<typename CharT, typename Traits, uint64_t, uint64_t A, uint64_t C, uint64_t M>
	std::basic_ostream<CharT, Traits>& operator<<( std::basic_ostream<CharT, Traits>& os, const linear_congruential_engine<uint64_t, A, C, M>& eng)
    {
        const auto sflags = os.flags();
        const auto sfill  = os.fill();
        
        os.flags(std::basic_istream<CharT, Traits>::ios_base::dec | std::basic_istream<CharT, Traits>::ios_base::fixed | std::basic_istream<CharT, Traits>::ios_base::left);
        os.fill(os.widen(' '));

        os << eng.get_seed();

        os.flags(sflags);
        os.fill(sfill);
        return os;
    }
}

