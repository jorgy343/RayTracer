#include <limits>
#include <cmath>

#include "Vcl.h"

export module RayTracer.AxisAlignedBox;

import RayTracer.Math;
import RayTracer.Geometry;
import RayTracer.Vector3;
import RayTracer.IntersectionResultType;

using namespace vcl;

namespace RayTracer
{
    export class alignas(16) AxisAlignedBox final : public Geometry
    {
    public:
        Vector3 Minimum{};
        Vector3 Maximum{};
        const LambertianMaterial* Material{nullptr};

        inline constexpr AxisAlignedBox() = default;

        inline constexpr AxisAlignedBox(const Vector3& minimum, const Vector3& maximum, const LambertianMaterial* material)
            : Minimum{minimum}, Maximum{maximum}, Material{material}
        {

        }

        inline constexpr const LambertianMaterial* GetMaterial() const override final
        {
            return Material;
        }

        constexpr Vector3 CalculateNormal(const Ray& ray, const Vector3& hitPosition) const override final
        {
            Vector3 distanceMinimum = (hitPosition - Minimum).Abs();
            Vector3 distanceMaxmimum = (hitPosition - Maximum).Abs();

            // -X
            Vector3 normal = Vector3(-1.0f, 0.0f, 0.0f);
            float minimumDistance = distanceMinimum.X;

            // +X  
            if (distanceMaxmimum.X < minimumDistance)
            {
                normal = Vector3(1.0f, 0.0f, 0.0f);
                minimumDistance = distanceMaxmimum.X;
            }

            // -Y  
            if (distanceMinimum.Y < minimumDistance)
            {
                normal = Vector3(0.0f, -1.0f, 0.0f);
                minimumDistance = distanceMinimum.Y;
            }

            // +Y  
            if (distanceMaxmimum.Y < minimumDistance)
            {
                normal = Vector3(0.0f, 1.0f, 0.0f);
                minimumDistance = distanceMaxmimum.Y;
            }

            // -Z  
            if (distanceMinimum.Z < minimumDistance)
            {
                normal = Vector3(0.0f, 0.0f, -1.0f);
                minimumDistance = distanceMinimum.Z;
            }

            // +Z  
            if (distanceMaxmimum.Z < minimumDistance)
            {
                normal = Vector3(0.0f, 0.0f, 1.0f);
            }

            return normal;
        }

        float IntersectEntrance(const Ray& ray) const override final
        {
            return Intersect<IntersectionResultType::Entrance>(ray);
        }

        float IntersectExit(const Ray& ray) const override final
        {
            return Intersect<IntersectionResultType::Exit>(ray);
        }

    private:
        template <IntersectionResultType TIntersectionResultType>
        inline float Intersect(const Ray& ray) const
        {
            Vec4f minimum = Vec4f{}.load(&Minimum.X);
            Vec4f maximum = Vec4f{}.load(&Maximum.X);
            Vec4f rayPosition = Vec4f{}.load(&ray.Position.X);
            Vec4f rayInverseDirection = Vec4f{}.load(&ray.InverseDirection.X);

            Vec4f temp = (maximum - rayPosition) * rayInverseDirection;

            Vec4f min = ConvertNanToInf((minimum - rayPosition) * rayInverseDirection);
            Vec4f max = ConvertNanToInf((maximum - rayPosition) * rayInverseDirection);

            Vec4f tempMax = vcl::max(min, max);
            tempMax.insert(3, std::numeric_limits<float>::max());

            float exitDistance = horizontal_min1(tempMax);

            if (exitDistance < 0.0f)
            {
                return std::numeric_limits<float>::infinity();
            }

            Vec4f tempMin = vcl::min(min, max);
            tempMin.insert(3, std::numeric_limits<float>::min());

            float entranceDistance = horizontal_max1(tempMin);

            float result;
            if constexpr (TIntersectionResultType == IntersectionResultType::Entrance)
            {
                result = entranceDistance;
            }
            else
            {
                result = exitDistance;
            }

            return entranceDistance <= exitDistance ? result : std::numeric_limits<float>::infinity();
        }
    };
}