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
import SignedDistance;

using namespace vcl;

namespace Yart
{
    export class __declspec(dllexport) alignas(32) AxisAlignedBox : virtual public Geometry, virtual public SignedDistance
    {
    private:
        using VclVec = typename std::conditional<std::same_as<real, float>, Vec4f, Vec4d>::type;

    public:
        Vector3 Minimum{};
        Vector3 Maximum{};
        const Material* AppliedMaterial{nullptr};

        inline AxisAlignedBox() = default;

        inline AxisAlignedBox(const Vector3& minimum, const Vector3& maximum, const Material* appliedMaterial)
            : Minimum{minimum}, Maximum{maximum}, AppliedMaterial{appliedMaterial}
        {

        }

        inline AxisAlignedBox(const BoundingBox& boundingBox, const Material* appliedMaterial)
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

        constexpr Vector3 CalculateNormal(const Ray& ray, const Vector3& hitPosition, real additionalData) const override
        {
            Vector3 distanceMinimum = (hitPosition - Minimum).Abs();
            Vector3 distanceMaxmimum = (hitPosition - Maximum).Abs();

            // -X
            Vector3 normal = Vector3(real{-1.0}, real{0.0}, real{0.0});
            real minimumDistance = distanceMinimum.X;

            // +X  
            if (distanceMaxmimum.X < minimumDistance)
            {
                normal = Vector3(real{1.0}, real{0.0}, real{0.0});
                minimumDistance = distanceMaxmimum.X;
            }

            // -Y  
            if (distanceMinimum.Y < minimumDistance)
            {
                normal = Vector3(real{0.0}, real{-1.0}, real{0.0});
                minimumDistance = distanceMinimum.Y;
            }

            // +Y  
            if (distanceMaxmimum.Y < minimumDistance)
            {
                normal = Vector3(real{0.0}, real{1.0}, real{0.0});
                minimumDistance = distanceMaxmimum.Y;
            }

            // -Z  
            if (distanceMinimum.Z < minimumDistance)
            {
                normal = Vector3(real{0.0}, real{0.0}, real{-1.0});
                minimumDistance = distanceMinimum.Z;
            }

            // +Z  
            if (distanceMaxmimum.Z < minimumDistance)
            {
                normal = Vector3(real{0.0}, real{0.0}, real{1.0});
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
        inline real Intersect(const Ray& ray) const
        {
            VclVec minimum = VclVec{}.load(&Minimum.X);
            VclVec maximum = VclVec{}.load(&Maximum.X);
            VclVec rayPosition = VclVec{}.load(&ray.Position.X);
            VclVec rayInverseDirection = VclVec{}.load(&ray.InverseDirection.X);

            VclVec temp = (maximum - rayPosition) * rayInverseDirection;

            VclVec min = ConvertNanToInf((minimum - rayPosition) * rayInverseDirection);
            VclVec max = ConvertNanToInf((maximum - rayPosition) * rayInverseDirection);

            VclVec tempMax = vcl::max(min, max);
            tempMax.insert(3, std::numeric_limits<real>::max());

            real exitDistance = horizontal_min1(tempMax);

            if (exitDistance < real{0.0})
            {
                return std::numeric_limits<real>::infinity();
            }

            VclVec tempMin = vcl::min(min, max);
            tempMin.insert(3, std::numeric_limits<real>::min());

            real entranceDistance = horizontal_max1(tempMin);

            real result;
            if constexpr (TIntersectionResultType == IntersectionResultType::Entrance)
            {
                result = entranceDistance;
            }
            else
            {
                result = exitDistance;
            }

            return entranceDistance <= exitDistance ? result : std::numeric_limits<real>::infinity();
        }

        SignedDistanceResult ClosestDistance(const Vector3& point) const override
        {
            // Source: https://stackoverflow.com/a/30545544/1078268
            Vector3 distance = Vector3::Max(Minimum - point, point - Maximum);

            return
            {
                (Vector3::Max(distance, Vector3{real{0}})).Length() + Math::min(Math::max(distance.X, Math::max(distance.Y, distance.Z)), real{0}),
                real{0},
            };
        }
    };
}