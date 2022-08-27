export module RayTracer.IntersectionResult;

import RayTracer.Vector3;

namespace RayTracer
{
    export template<typename TShape>
        class IntersectionResult
    {
    public:
        const TShape* Shape{nullptr};
        float Distance{0.0f};
        Vector3 HitPosition{};
        Vector3 Normal{};

        IntersectionResult(const TShape* shape, float distance, const Vector3& hitPosition, const Vector3& normal)
            : Shape{shape}, Distance{distance}, HitPosition{hitPosition}, Normal{normal}
        {

        }
    };
}