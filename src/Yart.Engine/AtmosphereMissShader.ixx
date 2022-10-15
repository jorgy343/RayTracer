export module AtmosphereMissShader;

import "Common.h";

import Math;
import MissShader;
import Random;
import Ray;
import Sphere;

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
        static constexpr real AtmosphericHeight{100'000};

        static constexpr Color3 Wavelength{real{6.80e-7}, real{5.50e-7}, real{4.40e-7}};
        static constexpr Color3 WavelengthPowFour = Wavelength * Wavelength * Wavelength * Wavelength;

        //static constexpr real G{-0.9};

        static constexpr real n{1.00031};
        static constexpr real Ns{2.55e25};
        static constexpr real Mu{0.75};
        static constexpr real Mx = (real{5} / real{9}) * Mu + (real{125} / real{729}) * Mu * Mu * Mu + Math::pow((real{64} / real{27} - (real{325} / real{243}) * Mu * Mu + (real{1250} / real{2187}) * Mu * Mu * Mu * Mu), real{0.5});
        //static constexpr real Mg = (real{5} / real{9}) * Mu - ((real{4} / real{3}) - (real{25} / real{81}) * Mu * Mu) * Math::pow(Mx, real{-1} / real{3}) + Math::pow(Mx, real{1} / real{3});
        static constexpr real Mg{0.85};

        static constexpr real Kr{(real{2} *Pi * Pi * (n * n - real{1}) * (n * n - real{1})) / (real{3} *Ns)};

        static constexpr Color3 Br{FourPi * Kr * Color3::Reciprical(WavelengthPowFour)};
        //static constexpr Color3 Br{real{5.8e-6}, real{13.5e-6}, real{33.1e-6}};
        static constexpr Color3 Bm{real{21e-6}};

        static constexpr Color3 SunIntensity{real{20.0}};
        static constexpr real Lambda{30};

    public:
        constexpr AtmosphereMissShader(const Vector3& sunDirection, const Vector3& origin)
            : SunDirectionReversed{-sunDirection}, Origin{origin}, Atmosphere{Vector3{}, EarthRadius + AtmosphericHeight, nullptr}
        {

        }

        Vector3 CalculateColor(const Ray& ray, const Random& random) const override
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

            // Calculate for the sun ray.
            Vector3 sunRayStart = viewRaySamplePoint;
            Ray sunRay{sunRayStart, SunDirectionReversed};
            real sunRayDistance = Atmosphere.IntersectExit(sunRay).HitDistance;

            Color3 sunRayOpticalDepth = OpticalDepth(sunRayStart, sunRayDistance, random);

            // Calculate final result.
            Color3 coefficient = (Br * viewRaySamplePointDensityR * RayleighPhase(cosTheta)) + (Bm * viewRaySamplePointDensityM * MiePhase(cosTheta));
            Color3 outScattering = (-sunRayOpticalDepth - viewRayOpticalDepth).Exp();

            Color3 result = SunIntensity * coefficient * outScattering * viewRayInversePdf;

            return static_cast<Vector3>(result);
        }

    protected:
        inline constexpr real Density(real altitude, real scaleHeight) const
        {
            return Math::exp(-altitude / scaleHeight);
        }

        inline constexpr real RayleighPhase(real cosTheta) const
        {
            //real coefficient = real{3} / real{4};
            real coefficient = real{3} / (real{16} * Pi);

            return coefficient * (real{1} + cosTheta * cosTheta);
        }

        inline constexpr real MiePhase(real cosTheta) const
        {
            //real coefficient{1.5};
            real coefficient = real{3} / EightPi;

            real numerator = (real{1} - Mg * Mg) * (real{1} + cosTheta * cosTheta);
            real denominator = (real{2} + Mg * Mg) * Math::pow(real{1} + Mg * Mg - real{2} *Mg * cosTheta, real{1.5});

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
    };
}