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
        Sphere Atmosphere{};

        real G{-0.9};
        Vector3 K{real{5.5e-6}, real{13.0e-6}, real{22.4e-6}};

        real AirAtmosphericDensity{7994.0};
        real AerosolAtmosphericDensity{1200.0};

        static constexpr real AtmosphericHeight{100'000};

        static constexpr Vector3 Wavelength{700, 510, 450};
        static constexpr Vector3 WavelengthPowFour = Wavelength.ComponentwiseMultiply(Wavelength).ComponentwiseMultiply(Wavelength).ComponentwiseMultiply(Wavelength);

    public:
        constexpr AtmosphereMissShader(const Vector3& sunDirection, const Vector3& origin)
            : SunDirectionReversed{-sunDirection}, Atmosphere{-origin, AtmosphericHeight, nullptr}
        {

        }

        Vector3 CalculateColor(const Ray& ray) const override
        {
            real viewRayDistance = Atmosphere.IntersectExit(ray).HitDistance;
            Vector3 viewRayPointOnAtmosphere = ray.Position + viewRayDistance * ray.Direction;

            constexpr unsigned int viewRaySegments = 10;
            constexpr unsigned int sunRaySegments = 5;

            Vector3 viewRaySegmentIncrement = (viewRayPointOnAtmosphere - ray.Position) / static_cast<real>(viewRaySegments);

            real result{};
            for (unsigned int viewRaySegmentIndex = 0; viewRaySegmentIndex < viewRaySegments; viewRaySegmentIndex++)
            {
                Vector3 viewRaySamplePoint = ray.Position + static_cast<real>(viewRaySegmentIndex) * viewRaySegmentIncrement;

                real viewRaySamplePointDensity = Density(viewRaySamplePoint.Length(), AirAtmosphericDensity);
                result += viewRaySamplePointDensity;

                // Handle the sun ray.
                Ray sunRay{viewRaySamplePoint, SunDirectionReversed};
                real sunRayDistance = Atmosphere.IntersectExit(sunRay).HitDistance;
                Vector3 sunRayPointOnAtmosphere = sunRay.Position + sunRayDistance * sunRay.Direction;

                Vector3 sunRaySegmentIncrement = (sunRayPointOnAtmosphere - sunRay.Position) / static_cast<real>(sunRaySegments);

                for (unsigned int sunRaySegmentIndex = 0; sunRaySegmentIndex < sunRaySegments; sunRaySegmentIndex++)
                {
                    Vector3 sunRaySamplePoint = sunRay.Position + static_cast<real>(sunRaySegmentIndex) * sunRay.Direction;

                    // TODO: Calculate sun ray sample calculation.
                }
            }

            result *= viewRayDistance / static_cast<real>(viewRaySegments);

            constexpr Vector3 sunIntensity{real{1.0}};

            Vector3 color = sunIntensity.ComponentwiseMultiply(K) * result;
            return color;
        }

    protected:
        inline constexpr real Density(real altitude, real scaleHeight) const
        {
            return Math::exp(-altitude / scaleHeight);
        }

        //inline constexpr real Phase(real theta) const
        //{
        //    return OneOverFourPi * ((real{1} - G * G) / Math::pow(real{1} + G * G - real{2} * G * Math::cos(theta), real{3.0 / 2.0}));
        //}

        //constexpr Vector3 Attenuation(const Vector3& point) const
        //{
        //    //Vector3 constant = WavelengthPowFour.RecipricalConst() * (-FourPi * K);


        //}
    };
}