export module AtmosphereMissShader;

import "Common.h";

import Math;
import MissShader;
import Ray;
import Sphere;

namespace Yart
{
    export class AtmosphereMissShader : public MissShader
    {
    protected:
        Vector3 SunDirectionReversed{};
        Vector3 Origin;
        Sphere Atmosphere{};

        real AirAtmosphericDensity{7994.0};
        real AerosolAtmosphericDensity{1200.0};

        static constexpr real AtmosphericHeight{real{100'000}};

        static constexpr Vector3 Wavelength{real{6.80e-7}, real{5.50e-7}, real{4.40e-7}};
        static constexpr Vector3 WavelengthPowFour = Wavelength.ComponentwiseMultiply(Wavelength).ComponentwiseMultiply(Wavelength).ComponentwiseMultiply(Wavelength);

        //static constexpr real G{-0.9};

        static constexpr real n{1.00031};
        static constexpr real Ns{2.55e25};
        static constexpr real Kr{(real{2} *Pi * Pi * (n * n - real{1}) * (n * n - real{1})) / (real{3} *Ns)};
        static constexpr Vector3 Br{FourPi * Kr * WavelengthPowFour.RecipricalConst()};

    public:
        constexpr AtmosphereMissShader(const Vector3& sunDirection, const Vector3& origin)
            : SunDirectionReversed{-sunDirection}, Origin{origin}, Atmosphere{Vector3{-origin}, AtmosphericHeight, nullptr}
        {

        }

        Vector3 CalculateColor(const Ray& ray) const override
        {
            real viewRayDistance = Atmosphere.IntersectExit(ray).HitDistance;
            Vector3 viewRayPointOnAtmosphere = ray.Position + viewRayDistance * ray.Direction;

            constexpr unsigned int viewRaySegments = 20;
            constexpr unsigned int sunRaySegments = 8;

            Vector3 viewRaySegmentIncrement = (viewRayPointOnAtmosphere - ray.Position) / static_cast<real>(viewRaySegments);

            real cosTheta = ray.Direction * -SunDirectionReversed;

            Vector3 viewRayRunningResult{};

            Vector3 result{};
            for (unsigned int viewRaySegmentIndex = 0; viewRaySegmentIndex < viewRaySegments; viewRaySegmentIndex++)
            {
                Vector3 viewRaySamplePoint = ray.Position + static_cast<real>(viewRaySegmentIndex) * viewRaySegmentIncrement;

                real viewRaySamplePointDensity = Density(viewRaySamplePoint.Length(), AirAtmosphericDensity);

                // Handle the sun ray.
                Ray sunRay{viewRaySamplePoint, SunDirectionReversed};
                real sunRayDistance = Atmosphere.IntersectExit(sunRay).HitDistance;
                Vector3 sunRayPointOnAtmosphere = sunRay.Position + sunRayDistance * sunRay.Direction;

                viewRayRunningResult += Density(viewRaySamplePoint.Length(), AirAtmosphericDensity);

                Vector3 sunRayOpticalDepth = OpticalDepth(viewRaySamplePoint, sunRayPointOnAtmosphere, sunRaySegments);

                Vector3 innerIntegral = -sunRayOpticalDepth - Br * viewRayRunningResult;
                innerIntegral = Vector3{Math::exp(innerIntegral.X), Math::exp(innerIntegral.Y), Math::exp(innerIntegral.Z)};

                result += viewRaySamplePointDensity * innerIntegral;
            }

            result *= viewRayDistance / static_cast<real>(viewRaySegments);

            constexpr Vector3 sunIntensity{real{20.0}};

            real viewRayRayleighPhaseFunction = RayleighPhase(cosTheta);
            Vector3 color = sunIntensity.ComponentwiseMultiply((Kr * viewRayRayleighPhaseFunction) * WavelengthPowFour.RecipricalConst()).ComponentwiseMultiply(result);

            return color;
        }

    protected:
        inline constexpr real Density(real altitude, real scaleHeight) const
        {
            return Math::exp(-altitude / scaleHeight);
        }

        inline constexpr real RayleighPhase(real cosTheta) const
        {
            return (real{3} / real{4}) * (real{1} *cosTheta * cosTheta);
        }

        inline constexpr Vector3 OpticalDepth(const Vector3& startingPoint, const Vector3& endingPoint, unsigned int segmentCount) const
        {
            real totalLength = (endingPoint - startingPoint).Length();
            real segmentLength = totalLength / static_cast<real>(segmentCount);

            Vector3 deltaSegment = (endingPoint - startingPoint).Normalize() * (totalLength / static_cast<real>(segmentCount));

            real result = Density(startingPoint.Length(), AirAtmosphericDensity) + Density(endingPoint.Length(), AirAtmosphericDensity);
            for (unsigned int segmentIndex = 1; segmentIndex < segmentCount - 1; segmentIndex++)
            {
                Vector3 point = startingPoint + static_cast<real>(segmentIndex) * deltaSegment;
                result = real{2} *Density(point.Length(), AirAtmosphericDensity);
            }

            result *= segmentLength * real{0.5};
            return Br * result;
        }
    };
}