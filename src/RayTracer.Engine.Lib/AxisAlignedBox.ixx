#include <limits>

#include "Vcl.h"

export module RayTracer.AxisAlignedBox;

import RayTracer.Math;
import RayTracer.Geometry;
import RayTracer.Vector3;

using namespace vcl;

namespace RayTracer
{
    export class alignas(16) AxisAlignedBox final : public Geometry
    {
    public:
        Vector3 Minimum{};
        Vector3 Maximum{};
        const LambertianMaterial* Material{nullptr};

        AxisAlignedBox() = default;

        AxisAlignedBox(const Vector3& minimum, const Vector3& maximum, const LambertianMaterial* material)
            : Minimum{minimum}, Maximum{maximum}, Material{material}
        {

        }

        inline const LambertianMaterial* GetMaterial() const override final
        {
            return Material;
        }

        inline Vector3 CalculateNormal(const Ray& ray, const Vector3& hitPosition) const override final
        {
            return Vector3{};
        }

        float Intersect(const Ray& ray) const override final
        {
            //Vector3 min = (Minimum - ray.Position).ComponentwiseMultiply(ray.InverseDirection);
            //Vector3 max = (Maximum - ray.Position).ComponentwiseMultiply(ray.InverseDirection);

            //float exitDistance = FastMin(FastMin(FastMax(min.X, max.X), FastMax(min.Y, max.Y)), FastMax(min.Z, max.Z));

            //// If tmax < 0, ray (line) is intersecting AABB, but whole AABB is behing us.
            //if (exitDistance < 0.0f)
            //{
            //    return std::numeric_limits<float>::infinity();
            //}

            //float entranceDistance = FastMax(FastMax(FastMax(FastMin(min.X, max.X), FastMin(min.Y, max.Y)), FastMin(min.Z, max.Z)), 0.0f);

            //// If tmin > tmax, ray doesn't intersect AABB.
            //return entranceDistance > exitDistance ? std::numeric_limits<float>::infinity() : entranceDistance;





            Vec4f minimum = Vec4f{}.load(&Minimum.X);
            Vec4f maximum = Vec4f{}.load(&Maximum.X);
            Vec4f rayPosition = Vec4f{}.load(&ray.Position.X);
            Vec4f rayInverseDirection = Vec4f{}.load(&ray.InverseDirection.X);

            Vec4f min = (minimum - rayPosition) * rayInverseDirection;
            Vec4f max = (maximum - rayPosition) * rayInverseDirection;

            Vec4f tempMax = vcl::max(min, max);
            tempMax.insert(3, std::numeric_limits<float>::max());

            float exitDistance = horizontal_min1(tempMax);

            if (exitDistance < 0.0f)
            {
                return std::numeric_limits<float>::infinity();
            }

            Vec4f tempMin = vcl::min(min, max);
            tempMin.insert(3, std::numeric_limits<float>::min());

            float entranceDistance = FastMax(horizontal_max1(tempMin), 0.0f);

            return entranceDistance > exitDistance ? std::numeric_limits<float>::infinity() : entranceDistance;
        }
    };
}