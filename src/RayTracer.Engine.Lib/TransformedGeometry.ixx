export module TransformedGeometry;

import IntersectableGeometry;
import Matrix4x4;

namespace RayTracer
{
    export class TransformedGeometry final : IntersectableGeometry
    {
    private:
        const IntersectableGeometry* _geometry;
        Matrix4x4 _inversedTransform;
        Matrix4x4 _inverseTransposedTransform;

    public:
        constexpr TransformedGeometry(const IntersectableGeometry* geometry, const Matrix4x4& transform)
            : _geometry{geometry}, _inversedTransform{transform.InvertConst()}, _inverseTransposedTransform{transform.InvertConst().TransposeConst()}
        {

        }

        constexpr IntersectionResult IntersectEntrance(const Ray& ray) const override final
        {
            // TODO: Implement.
            return _geometry->IntersectEntrance(ray);
        }

        constexpr IntersectionResult IntersectExit(const Ray& ray) const override final
        {
            // TODO: Implement.
            return _geometry->IntersectExit(ray);
        }
    };
}