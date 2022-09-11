export module RayTracer.ParallelogramLight;

import RayTracer.Light;
import RayTracer.Vector3;
import RayTracer.Ray;
import RayTracer.Parallelogram;
import RayTracer.Random;
import RayTracer.LightRay;

namespace RayTracer
{
    export class ParallelogramLight : public Light
    {
    private:
        Random _random{};

    public:
        Vector3 Color{};
        Parallelogram Shape{};

        inline constexpr ParallelogramLight(const Vector3& color, const Parallelogram& shape)
            : Color{color}, Shape{shape}
        {

        }

        inline constexpr LightRay CreateShadowRaw(const Vector3& hitPosition, const Vector3& hitNormal) const override final
        {
            Vector3 randomPointOnLight = Shape.Position + (Shape.Edge1 * _random.GetNormalizedFloat()) + (Shape.Edge2 * _random.GetNormalizedFloat());
            Vector3 directionToLight = (randomPointOnLight - hitPosition).Normalize();

            return LightRay{Ray{hitPosition, directionToLight}, randomPointOnLight};
        }

        inline constexpr Vector3 CalculateLightPower(const Vector3& hitPosition, const Vector3& hitNormal, const Vector3& pointOnLight, float shadowDistance) const override final
        {
            Vector3 directionToLight = pointOnLight - hitPosition;
            float distanceToLightSquared = directionToLight.LengthSquared();

            directionToLight.Normalize();

            if (shadowDistance * shadowDistance >= distanceToLightSquared - 0.01f)
            {
                return Color * Math::max(0.0f, hitNormal * directionToLight);
            }

            return Vector3{};
        }
    };
}