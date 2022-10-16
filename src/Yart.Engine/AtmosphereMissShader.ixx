module;

#include "Vcl.h"

export module AtmosphereMissShader;

import "Common.h";

import Math;
import MissShader;
import Random;
import Ray;
import Sphere;

using namespace vcl;

namespace Yart
{
    export class AtmosphereMissShader : public MissShader
    {
    protected:
        Vector3 SunDirectionReversed{};
        Vector3 Origin{};
        Sphere Atmosphere{};

        real AirAtmosphericDensity{7994.0};
        real AerosolAtmosphericDensity{1200.0};

        static constexpr real EarthRadius{6'371'071};
        static constexpr real AtmosphericHeight{60'000};

        static constexpr Color3 Wavelength{real{6.80e-7}, real{5.50e-7}, real{4.40e-7}};
        static constexpr Color3 WavelengthPowFour = Wavelength * Wavelength * Wavelength * Wavelength;

        //static constexpr real G{-0.9};

        static constexpr real n{1.00031};
        static constexpr real Ns{2.55e25};

        static constexpr real u{0.8};
        static constexpr real x = (real{5} / real{9}) * u + (real{125} / real{729}) * u * u * u + Math::sqrt(real{64} / real{27} - (real{325} / real{243}) * u * u + (real{1250} / real{2187}) * u * u * u * u);
        static constexpr real g = (real{5} / real{9}) * u - ((real{4} / real{3}) - (real{25} / real{81}) * u * u) * Math::pow(x, real{-1} / real{3}) + Math::pow(x, real{1} / real{3});

        static constexpr real Kr{(real{2} *Pi * Pi * (n * n - real{1}) * (n * n - real{1})) / (real{3} *Ns)};

        //static constexpr Color3 Br{FourPi * Kr * Color3::Reciprical(WavelengthPowFour)};
        static constexpr Color3 Br{real{5.8e-6}, real{13.5e-6}, real{33.1e-6}};
        static constexpr Color3 Bm{real{21e-6}};

        static constexpr Color3 SunIntensity{real{20.0}};
        static constexpr real Lambda{20};

    public:
        constexpr AtmosphereMissShader(const Vector3& sunDirection, const Vector3& origin)
            : SunDirectionReversed{-sunDirection}, Origin{origin}, Atmosphere{Vector3{}, EarthRadius + AtmosphericHeight, nullptr}
        {

        }

        //Color3 CalculateColorVec(const Ray& ray, const Random& random) const
        //{
        //    VectorVec3<real_vec> rayDirection{ray.Direction};
        //    VectorVec3<real_vec> sunDirectionReversed{SunDirectionReversed};

        //    real_vec cosTheta = VectorVec3<real_vec>::Dot(rayDirection, sunDirectionReversed);

        //    // Calculate view ray.
        //    real viewRayDistance = Atmosphere.IntersectExit(ray).HitDistance;
        //    Vector3 viewRayStart = ray.Position;
        //    Vector3 viewRayEnd = viewRayStart + viewRayDistance * ray.Direction;

        //    auto [viewRayRandomNumber, viewRayInversePdf] = random.GetExponentialRandomAndInversePdf(viewRayDistance, Lambda);

        //    real viewRaySampleDistance = viewRayRandomNumber * viewRayDistance;
        //    Vector3 viewRaySamplePoint = ray.Position + viewRaySampleDistance * ray.Direction;
        //    real viewRaySamplePointHeight = viewRaySamplePoint.Length() - EarthRadius;

        //    real viewRaySamplePointDensityR = Density(viewRaySamplePointHeight, AirAtmosphericDensity);
        //    real viewRaySamplePointDensityM = Density(viewRaySamplePointHeight, AerosolAtmosphericDensity);

        //    Color3 viewRayOpticalDepth = OpticalDepth(viewRayStart, viewRaySampleDistance, random);

        //    // Calculate for the sun ray.
        //    Vector3 sunRayStart = viewRaySamplePoint;
        //    Ray sunRay{sunRayStart, SunDirectionReversed};
        //    real sunRayDistance = Atmosphere.IntersectExit(sunRay).HitDistance;

        //    Color3 sunRayOpticalDepth = OpticalDepth(sunRayStart, sunRayDistance, random);

        //    // Calculate final result.
        //    Color3 coefficient = (Br * viewRaySamplePointDensityR * RayleighPhase(cosTheta)) + (Bm * viewRaySamplePointDensityM * MiePhase(cosTheta));
        //    Color3 outScattering = (-sunRayOpticalDepth - viewRayOpticalDepth).Exp();

        //    Color3 result = SunIntensity * coefficient * outScattering * viewRayInversePdf;
        //    return result;
        //}

        Color3 CalculateColor(const Ray& ray, const Random& random) const override
        {
            real cosTheta = ray.Direction * SunDirectionReversed;

            // Calculate view ray.
            real viewRayDistance = Atmosphere.IntersectExit(ray).HitDistance;
            Vector3 viewRayStart = ray.Position;
            Vector3 viewRayEnd = viewRayStart + viewRayDistance * ray.Direction;

            auto [viewRayRandomNumber, viewRayInversePdf] = random.GetExponentialRandomAndInversePdf(viewRayDistance, Lambda);

            real viewRaySampleDistance = viewRayRandomNumber * viewRayDistance;
            Vector3 viewRaySamplePoint = ray.Position + viewRaySampleDistance * ray.Direction;
            real viewRaySamplePointHeight = viewRaySamplePoint.Length() - EarthRadius;

            real viewRaySamplePointDensityR = Density(viewRaySamplePointHeight, AirAtmosphericDensity);
            real viewRaySamplePointDensityM = Density(viewRaySamplePointHeight, AerosolAtmosphericDensity);

            Color3 viewRayOpticalDepth = OpticalDepth(viewRayStart, viewRaySampleDistance, random);
            //Color3 viewRayOpticalDepth = OpticalDepthTrapazoidal(viewRayStart, ray.Direction, viewRaySampleDistance, random);

            // Calculate for the sun ray.
            Vector3 sunRayStart = viewRaySamplePoint;
            Ray sunRay{sunRayStart, SunDirectionReversed};
            real sunRayDistance = Atmosphere.IntersectExit(sunRay).HitDistance;

            Color3 sunRayOpticalDepth = OpticalDepth(sunRayStart, sunRayDistance, random);
            //Color3 sunRayOpticalDepth = OpticalDepthTrapazoidal(sunRayStart, sunRay.Direction, sunRayDistance, random);

            // Calculate final result.
            Color3 coefficient = (Br * viewRaySamplePointDensityR * RayleighPhase(cosTheta)) + (Bm * viewRaySamplePointDensityM * MiePhase(cosTheta));
            Color3 outScattering = (-sunRayOpticalDepth - viewRayOpticalDepth).Exp();

            Color3 result = SunIntensity * coefficient * outScattering * viewRayInversePdf;
            return result;
        }

    protected:
        inline constexpr real Density(real altitude, real scaleHeight) const
        {
            return Math::exp(-altitude / scaleHeight);
        }

        inline constexpr real RayleighPhase(real cosTheta) const
        {
            //real coefficient = real{3} / real{4};
            real coefficient = real{3} / (real{16} *Pi);

            return coefficient * (real{1} + cosTheta * cosTheta);
        }

        inline constexpr real MiePhase(real cosTheta) const
        {
            //real coefficient{1.5};
            real coefficient = real{3} / EightPi;

            real numerator = (real{1} - g * g) * (real{1} + cosTheta * cosTheta);
            real denominator = (real{2} + g * g) * Math::pow(real{1} + g * g - real{2} *g * cosTheta, real{1.5});

            return coefficient * (numerator / denominator);
        }

        inline Color3 OpticalDepth(const Vector3& startingPoint, real distance, const Random& random) const
        {
            auto [randomNumber, inversePdf] = random.GetExponentialRandomAndInversePdf(distance, Lambda);

            Vector3 samplePoint = startingPoint + randomNumber * distance * SunDirectionReversed;
            real samplePointHeight = samplePoint.Length() - EarthRadius;

            real densityR = Density(samplePointHeight, AirAtmosphericDensity);
            real densityM = Density(samplePointHeight, AerosolAtmosphericDensity);

            return ((densityR * Br) + (densityM * Bm)) * inversePdf;
        }

        inline Color3 OpticalDepthTrapazoidal(const Vector3& startingPoint, const Vector3& direction, real distance, const Random& random) const
        {
            constexpr int sampleCount = 8;

            Vector3 segmentIncrement = direction * (distance / static_cast<real>(sampleCount));

            real resultR{0};
            real resultM{0};

            real firstSampleHeight = startingPoint.Length() - EarthRadius;
            real lastSampleHeight = (startingPoint + direction * distance).Length() - EarthRadius;

            resultR += Density(firstSampleHeight, AirAtmosphericDensity);
            resultM += Density(firstSampleHeight, AerosolAtmosphericDensity);

            resultR += Density(lastSampleHeight, AirAtmosphericDensity);
            resultM += Density(lastSampleHeight, AerosolAtmosphericDensity);

            for (int i = 1; i < sampleCount - 1; i++)
            {
                real samplePointHeight = (startingPoint + static_cast<real>(i) * segmentIncrement).Length() - EarthRadius;

                resultR += Density(samplePointHeight, AirAtmosphericDensity);
                resultM += Density(samplePointHeight, AerosolAtmosphericDensity);
            }

            real integrationThing = distance / static_cast<real>(sampleCount) / real{2};

            resultR *= integrationThing;
            resultM *= integrationThing;

            return resultR * Br + resultM + Bm;
        }

        //inline Color3 OpticalDepthVec(const Vector3& startingPoint, real distance, const Random& random) const
        //{
        //    auto u = random.GetNormalizedVec();
        //    auto x = random.ExponentialRandomVec(u, Lambda);
        //    auto inversePdf = random.ExponentialRandomPdfVec(u, Lambda);

        //    return Color3{};
        //}
    };
}