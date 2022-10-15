module;

#include "Vcl.h"

export module Random;

import <random>;

import "Common.h";

import Math;

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

        template <real_number T = real>
        inline T GetNormalized() const
        {
            if constexpr (std::same_as<float, T>)
            {
                return _ranvec.random1f();
            }
            else
            {
                return _ranvec.random1d();
            }
        }

        template <real_number T = real>
        inline auto GetNormalizedVec() const
        {
            if constexpr (std::same_as<float, T>)
            {
                return _ranvec.random8f();
            }
            else
            {
                return _ranvec.random4d();
            }
        }

        template <real_number T = real>
        inline std::tuple<T, T> GetExponentialRandomAndInversePdf(T distance, T lambda) const
        {
            T u = GetNormalized<T>();
            T x = ExponentialRandom(u, lambda);
            T inversePdf = distance * ExponentialRandomPdf(u, lambda);

            return std::make_tuple(x, inversePdf);
        }

        RealVec ExponentialRandomVec(RealVec u, real lambda) const
        {
            return log(RealVec{real{1}} - (RealVec{real{1}} - exp(-RealVec{lambda})) * u) / RealVec{lambda};
        }

        RealVec ExponentialRandomPdfVec(RealVec u, real lambda) const
        {
            return (RealVec{real{1}} - exp(RealVec{lambda})) / (RealVec{lambda} *exp(RealVec{lambda}) * (u - RealVec{real{1}}) - RealVec{lambda} *u);
        }

    private:
        template <real_number T = real>
        force_inline constexpr T ExponentialRandom(T u, T lambda) const
        {
            return -Math::log(T{1} - (T{1} - Math::exp(-lambda)) * u) / lambda;
        }

        template <real_number T = real>
        force_inline constexpr T ExponentialRandomPdf(T u, T lambda) const
        {
            return (T{1} - Math::exp(lambda)) / (lambda * Math::exp(lambda) * (u - T{1}) - lambda * u);
        }
    };
}