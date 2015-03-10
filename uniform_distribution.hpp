// -*- C++ -*-

#pragma once

#include <utility>

namespace std
{
    template <typename TRealType = double> class uniform_distribution
    {
        static_assert(std::is_floating_point<TRealType>::value, "template argument not a floating point type");        
    
#pragma region Typedefs/Constants
        public: using result_type = TRealType;
        public: using param_type  = std::pair<TRealType, TRealType>;        
#pragma endregion

#pragma region Data
#pragma endregion

#pragma region Ctor/Dtor/op=
        public: explicit uniform_distribution() = default;        
#pragma endregion

#pragma region Observers
        public: constexpr result_type a() const
        {
            return TRealType(0);
        }

        public: constexpr result_type b() const
        {
            return TRealType(1);
        }

        public: constexpr param_type param() const
        {
            return param_type(TRealType(0), TRealType(1));
        }

        public: constexpr result_type min() const
        {
            return this->a();
        }

        public: constexpr result_type max() const
        {
            return this->b();
        }
#pragma endregion

#pragma region Mutators
        template <typename TURNG>	result_type	operator()(TURNG& urng)
        {
            return TRealType(urng()) / TRealType(TURNG::modulus);
        }

        public: void reset()
        {
        }
        
        public: void param(const param_type&)
        {
        }
#pragma endregion
    };
}
