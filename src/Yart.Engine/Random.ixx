module;

#include "Vcl.h"

export module Random;

import <cstdint>;
import <random>;

using namespace vcl;

namespace Yart
{
    export class Random
    {
    private:
        mutable uint32_t _state{};
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
    };
}