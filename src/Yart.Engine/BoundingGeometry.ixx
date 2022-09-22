export module BoundingGeometry;

import <vector>;

import "Common.h";

import IntersectableGeometry;
import IntersectionResultType;
import Math;

namespace Yart
{
    export class BoundingGeometry : public IntersectableGeometry
    {
    protected:
        const IntersectableGeometry* BoundingVolume{};
        const IntersectableGeometry* ChildGeometry{};

    public:
        inline constexpr BoundingGeometry(
            const IntersectableGeometry* boundingVolumne,
            const IntersectableGeometry* childGeometry)
            :
            BoundingVolume{boundingVolumne},
            ChildGeometry{childGeometry}
        {

        }

        inline IntersectionResult IntersectEntrance(const Ray& ray) const override
        {
            return Intersect<IntersectionResultType::Entrance>(ray);
        }

        inline IntersectionResult IntersectExit(const Ray& ray) const override
        {
            return Intersect<IntersectionResultType::Exit>(ray);
        }

    private:
        template <IntersectionResultType TIntersectionResultType>
        force_inline IntersectionResult Intersect(const Ray& ray) const
        {
            IntersectionResult result = BoundingVolume->IntersectExit(ray);
            if (Math::isfinite(result.HitDistance))
            {
                IntersectionResult childResult;
                if constexpr (TIntersectionResultType == IntersectionResultType::Entrance)
                {
                    childResult = ChildGeometry->IntersectEntrance(ray);
                }
                else
                {
                    childResult = ChildGeometry->IntersectExit(ray);
                }

                return childResult;
            }

            return result;
        }
    };
}