export module IntersectionResult;

import "Common.h";

import GeometryDecl;
import Material;

namespace Yart
{
    export template <real_number T>
    class IntersectionResultT
    {
    public:
        const Geometry* HitGeometry{};
        T HitDistance{};
        T AdditionalData{};
        const Material* MaterialOverride{};

		IntersectionResultT() = default;

        IntersectionResultT(const Geometry* hitGeometry, T hitDistance, T additionalData = {}, const Material* materialOverride = nullptr)
            : HitGeometry{hitGeometry}, HitDistance{hitDistance}, AdditionalData{additionalData}, MaterialOverride{materialOverride}
        {
            
        }
    };

    export using IntersectionResult = IntersectionResultT<real>;
}