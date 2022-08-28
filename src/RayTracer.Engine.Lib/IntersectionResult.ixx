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

        IntersectionResult(const TShape* shape, float distance)
            : Shape{shape}, Distance{distance}
        {

        }
    };
}