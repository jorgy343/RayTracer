export module SignedDistance;

import "Common.h";

import BoundingBox;
import Geometry;
import Material;
import Math;
import SignedDistanceResult;

namespace Yart
{
    export class __declspec(dllexport) SignedDistance
    {
    public:
        virtual BoundingBox CalculateBoundingBox() const
        {
            return BoundingBox{
                Vector3{-std::numeric_limits<real>::infinity()},
                Vector3{std::numeric_limits<real>::infinity()},
            };
        }

        virtual const Material* GetMaterial() const = 0;
        virtual SignedDistanceResult ClosestDistance(const Vector3& point) const = 0;
    };
}