module;

export module IntersectionResult;

namespace Yart
{
    export class Geometry;

    export class IntersectionResult
    {
    public:
        const Geometry* HitGeometry{nullptr};
        float HitDistance{0.0f};

		inline constexpr IntersectionResult() = default;

        inline constexpr IntersectionResult(const Geometry* hitGeometry, float hitDistance)
            : HitGeometry{hitGeometry}, HitDistance{hitDistance}
        {
            
        }
    };
}