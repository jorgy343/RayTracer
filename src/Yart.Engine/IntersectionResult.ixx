export module IntersectionResult;

import "Common.h";

import GeometryForward;

namespace Yart
{
    export template <real_number T>
    class IntersectionResultT
    {
    public:
        const Geometry* HitGeometry{nullptr};
        T HitDistance{0.0f};

		inline constexpr IntersectionResultT() = default;

        inline IntersectionResultT(const Geometry* hitGeometry, T hitDistance)
            : HitGeometry{hitGeometry}, HitDistance{hitDistance}
        {
            
        }
    };

    export using IntersectionResult = IntersectionResultT<real>;
}