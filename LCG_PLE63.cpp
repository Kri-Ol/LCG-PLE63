#include "LCG_PLE63.hpp"

namespace OTI
{
    // In cases where we want to skip backwards, we add the period of the random
    // number generator until the number of random numbers to skip is positive since
    // skipping ahead that much is the same as skipping backwards by the original amount
    int64_t lcg_PLE63::compute_nskip(int64_t ns)
    {
        int64_t nskip = ns;
        while (nskip < 0LL)
        {
            uint64_t t = uint64_t(nskip) + mod;
            nskip = int64_t(t);
        }
        return nskip & mask; // keep it inside our bit-range
    }

    lcg_PLE63::result_type lcg_PLE63::skip(int64_t ns, seed_type seed)
    {
        // compute positive number of seeds to skip
        int64_t nskip = compute_nskip(ns);

        // The algorithm here to determine the parameters used to skip ahead is
        // described in the paper F. Brown, "Random Number Generation with Arbitrary Stride,"
        // Trans. Am. Nucl. Soc. (Nov. 1994). This algorithm is able to skip ahead in
        // O(log2(N)) operations instead of O(N). It computes parameters
        // M and C which can then be used to find x_N = M*x_0 + C mod 2^M.

        // initialize constants
        result_type m{ mult }; // original multiplicative constant
        result_type c{ add };  // original additive constant

        result_type m_next{ 1ULL }; // new effective multiplicative constant
        result_type c_next{ 0ULL }; // new effective additive constant

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

        // with G and C, we can now find the new seed
        return (m_next * seed + c_next) & mask;
    }
}
