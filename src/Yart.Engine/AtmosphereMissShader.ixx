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

        static constexpr real AtmosphericHeight{real{100'000}};

        static constexpr Vector3 Wavelength{real{6.80e-7}, real{5.50e-7}, real{4.40e-7}};
        static constexpr Vector3 WavelengthPowFour = Vector3::ComponentwiseMultiply(Wavelength, Vector3::ComponentwiseMultiply(Wavelength, Vector3::ComponentwiseMultiply(Wavelength, Wavelength)));

        //static constexpr real G{-0.9};

        static constexpr real n{1.00031};
        static constexpr real Ns{2.55e25};
        static constexpr real Kr{(real{2} *Pi * Pi * (n * n - real{1}) * (n * n - real{1})) / (real{3} *Ns)};

        static constexpr Vector3 Br{FourPi * Kr * Vector3::Reciprical(WavelengthPowFour)};
        static constexpr Vector3 Bm{FourPi * Kr};

        static constexpr Vector3 SunIntensity{real{50.0}};
        static constexpr real Lambda{8};

    public:
        constexpr AtmosphereMissShader(const Vector3& sunDirection, const Vector3& origin)
            : SunDirectionReversed{-sunDirection}, Origin{origin}, Atmosphere{Vector3{-origin}, AtmosphericHeight, nullptr}
        {

        }

        Vector3 CalculateColor(const Ray& ray, const Random& random) const override
        {
            real viewRayDistance = Atmosphere.IntersectExit(ray).HitDistance;
            Vector3 viewRayPointOnAtmosphere = ray.Position + viewRayDistance * ray.Direction;

            real cosTheta = ray.Direction * SunDirectionReversed;
            real viewRayRayleighPhaseFunction = RayleighPhase(cosTheta);

            real u = random.GetNormalized();
            real x = ExponentialRandom(u, Lambda);
            real inverseViewRayPdf = viewRayDistance * ExponentialPdf(u, Lambda);

            Vector3 viewRaySamplePoint = ray.Position + x * viewRayDistance * ray.Direction;
            real viewRaySamplePointDensity = Density(viewRaySamplePoint.Length(), AirAtmosphericDensity);

            Ray sunRay{viewRaySamplePoint, SunDirectionReversed};
            real sunRayDistance = Atmosphere.IntersectExit(sunRay).HitDistance;
            Vector3 sunRayPointOnAtmosphere = sunRay.Position + sunRayDistance * sunRay.Direction;

            real viewRayOpticalDepth = OpticalDepth(ray.Position, viewRaySamplePoint, random);
            real sunRayOpticalDepth = OpticalDepth(viewRaySamplePoint, sunRayPointOnAtmosphere, random);

            Vector3 opticalDepth = Br * (-sunRayOpticalDepth - viewRayOpticalDepth);
            opticalDepth = Vector3{Math::exp(opticalDepth.X), Math::exp(opticalDepth.Y), Math::exp(opticalDepth.Z)};

            Vector3 integralResult = viewRaySamplePointDensity * opticalDepth * inverseViewRayPdf;
            Vector3 result = Vector3::ComponentwiseMultiply(Vector3::ComponentwiseMultiply(SunIntensity, (Kr * viewRayRayleighPhaseFunction) * Vector3::Reciprical(WavelengthPowFour)), integralResult);

            return result;
        }

    protected:
        inline constexpr real Density(real altitude, real scaleHeight) const
        {
            return Math::exp(-altitude / scaleHeight);
        }

        inline constexpr real RayleighPhase(real cosTheta) const
        {
            return (real{3} / real{4}) * (real{1} + cosTheta * cosTheta);
        }

        inline real OpticalDepth(const Vector3& startingPoint, const Vector3& endingPoint, const Random& random) const
        {
            real rayDistance = (endingPoint - startingPoint).Length();

            real u = random.GetNormalized();
            real x = ExponentialRandom(u, Lambda);
            real inversePdf = rayDistance * ExponentialPdf(u, Lambda);

            Vector3 samplePoint = startingPoint + x * rayDistance * (endingPoint - startingPoint).Normalize();
            real density = Density(samplePoint.Length(), AirAtmosphericDensity);

            return density * inversePdf;
        }

        force_inline constexpr real ExponentialRandom(real u, real lambda) const
        {
            return -Math::log(real{1} - (real{1} - Math::exp(-lambda)) * u) / lambda;
        }

        force_inline constexpr real ExponentialPdf(real u, real lambda) const
        {
            return (real{1} - Math::exp(lambda)) / (lambda * Math::exp(lambda) * (u - real{1}) - lambda * u);
        }
    };
}