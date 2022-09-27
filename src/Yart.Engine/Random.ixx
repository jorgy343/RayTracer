module;

#include "Vcl.h"

export module Random;

import <concepts>;
import <cstdint>;
import <random>;

import "Common.h";

using namespace vcl;

namespace Yart
{
    export class Random
    {
    private:
        mutable Ranvec1 _ranvec{3};

    public:
        Random()
        {
            std::random_device randomDevice{};
            _ranvec.init(physicalSeed(), randomDevice());
        }

        inline int GetInteger(int inclusiveMin, int inclusiveMax) const
        {
            return _ranvec.random1i(inclusiveMin, inclusiveMax);
        }

        inline float GetNormalizedFloat() const
        {
            return _ranvec.random1f();
        }

        template <real_number T>
        inline T GetNormalized() const
        {
            if constexpr (std::same_as<float, T>)
            {
                return _ranvec.random1f();
            }
            else if constexpr (std::same_as<double, T>)
            {
                return _ranvec.random1d();
            }
            else
            {
                static_assert("Type not supported in GetNormalized().");
            }
        }
    };
}