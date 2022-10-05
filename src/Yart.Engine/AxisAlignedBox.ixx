module;

#include "Vcl.h"

export module AxisAlignedBox;

import "Common.h";

import BoundingBox;
import Geometry;
import IntersectionResult;
import IntersectionResultType;
import Material;
import Math;

using namespace vcl;

namespace Yart
{
    export class __declspec(dllexport) alignas(16) AxisAlignedBox : public Geometry
    {
    public:
        Vector3 Minimum{};
        Vector3 Maximum{};
        const Material* AppliedMaterial{nullptr};

        inline constexpr AxisAlignedBox() = default;

        inline constexpr AxisAlignedBox(const Vector3& minimum, const Vector3& maximum, const Material* appliedMaterial)
            : Minimum{minimum}, Maximum{maximum}, AppliedMaterial{appliedMaterial}
        {

        }

        inline constexpr AxisAlignedBox(const BoundingBox& boundingBox, const Material* appliedMaterial)
            : Minimum{boundingBox.Minimum}, Maximum{boundingBox.Maximum}, AppliedMaterial{appliedMaterial}
        {

        }

        constexpr BoundingBox CalculateBoundingBox() const override
        {
            return BoundingBox{Minimum, Maximum};
        }

        inline constexpr const Material* GetMaterial() const override
        {
            return AppliedMaterial;
        }

        constexpr Vector3 CalculateNormal(const Ray& ray, const Vector3& hitPosition, float additionalData) const override
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

        IntersectionResult IntersectEntrance(const Ray& ray) const override
        {
            return {this, Intersect<IntersectionResultType::Entrance>(ray)};
        }

        IntersectionResult IntersectExit(const Ray& ray) const override
        {
            return {this, Intersect<IntersectionResultType::Exit>(ray)};
        }

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