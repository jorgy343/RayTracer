export module IntersectionResult;

import "Common.h";

import GeometryDecl;

namespace Yart
{
    export template <real_number T>
    class IntersectionResultT
    {
    public:
        const Geometry* HitGeometry{};
        T HitDistance{};
        T AdditionalData{};

		inline constexpr IntersectionResultT() = default;

        inline IntersectionResultT(const Geometry* hitGeometry, T hitDistance, T additionalData = {})
            : HitGeometry{hitGeometry}, HitDistance{hitDistance}, AdditionalData{additionalData}
        {
            
        }
    };

    export using IntersectionResult = IntersectionResultT<real>;
}