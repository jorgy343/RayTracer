export module BoundingGeometry;

import "Common.h";

import AxisAlignedBox;
import BoundingBox;
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
            const IntersectableGeometry* boundingVolume,
            const IntersectableGeometry* childGeometry)
            :
            BoundingVolume{boundingVolume},
            ChildGeometry{childGeometry}
        {

        }

        constexpr BoundingBox CalculateBoundingBox() const override
        {
            return BoundingVolume->CalculateBoundingBox();
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

    export const BoundingGeometry* CreateBoundingGeometryFromGeometry(const IntersectableGeometry* geometry, std::vector<std::shared_ptr<const IntersectableGeometry>>& geometryPointers)
    {
        auto boundingBox = geometry->CalculateBoundingBox().AddMargin(Vector3{0.01f});
        auto axisAlignedBox = std::make_shared<const AxisAlignedBox>(boundingBox, nullptr);

        auto boundingGeometry = std::make_shared<const BoundingGeometry>(axisAlignedBox.get(), geometry);

        geometryPointers.push_back(axisAlignedBox);
        geometryPointers.push_back(boundingGeometry);

        return boundingGeometry.get();
    }
}