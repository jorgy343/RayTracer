#include <cmath>
#include <concepts>
#include <utility>

#include "Constants.h"

export module RayTracer.Scene;

import RayTracer.Alignment;
import RayTracer.Light;
import RayTracer.LambertianMaterial;
import RayTracer.Math;
import RayTracer.MonteCarlo;
import RayTracer.Sphere;
import RayTracer.SphereSoa;
import RayTracer.Plane;
import RayTracer.PlaneSoa;
import RayTracer.AxisAlignedBox;
import RayTracer.AxisAlignedBoxSoa;
import RayTracer.Parallelogram;
import RayTracer.ParallelogramSoa;
import RayTracer.Geometry;
import RayTracer.GeometrySoa;
import RayTracer.Vector3;
import RayTracer.Random;
import RayTracer.LightRay;

namespace RayTracer
{
    export class Scene
    {
    private:
        Vector3 _backgroundColor{0.0f};

        AlignedVector<const Light*, 64> _lights{};

        SphereSoa _sphereSoa{};
        PlaneSoa _planeSoa{};
        AxisAlignedBoxSoa _axisAlignedBoxSoa{};
        ParallelogramSoa _parallelogramSoa{};

        Random _random{};

    public:
        explicit Scene(Vector3 backgroundColor)
            : _backgroundColor{backgroundColor}
        {

        }

        void AddLight(const Light* light)
        {
            _lights.push_back(light);
        }

        void AddGeometry(const Sphere* sphere)
        {
            _sphereSoa.Add(sphere);
        }

        void AddGeometry(const Plane* plane)
        {
            _planeSoa.Add(plane);
        }

        void AddGeometry(const AxisAlignedBox* axisAlignedBox)
        {
            _axisAlignedBoxSoa.Add(axisAlignedBox);
        }

        void AddGeometry(const Parallelogram* parallelogram)
        {
            _parallelogramSoa.Add(parallelogram);
        }

        void Finalize()
        {
            _sphereSoa.Finalize();
            _planeSoa.Finalize();
            _axisAlignedBoxSoa.Finalize();
            _parallelogramSoa.Finalize();
        }

        Vector3 CastRayColor(const Ray& ray) const
        {
            return CastRayColor(ray, 1);
        }

    private:
        template <typename TArg, typename... TArgs>
        static inline constexpr auto ClosestIntersection(TArg arg, TArgs... args)
        {
            if constexpr (sizeof...(args))
            {
                auto smallerIntersection = ClosestIntersection(args...);
                return arg.Distance < smallerIntersection.Distance ? arg : smallerIntersection;
            }
            else
            {
                return arg;
            }
        }

        float CastRayDistance(const Ray& ray) const
        {
            auto closestIntersection = ClosestIntersection(
                _sphereSoa.IntersectEntrance(ray),
                _planeSoa.IntersectEntrance(ray),
                _axisAlignedBoxSoa.IntersectEntrance(ray),
                _parallelogramSoa.IntersectEntrance(ray));

            return Math::max(0.0f, closestIntersection.Distance);
        }

        Vector3 CastRayColor(const Ray& ray, int depth) const
        {
            auto closestIntersection = ClosestIntersection(
                _sphereSoa.IntersectEntrance(ray),
                _planeSoa.IntersectEntrance(ray),
                _axisAlignedBoxSoa.IntersectEntrance(ray),
                _parallelogramSoa.IntersectEntrance(ray));

            closestIntersection.Distance = Math::max(0.0f, closestIntersection.Distance);

            Vector3 outputColor = _backgroundColor;

            if (closestIntersection.HitGeometry)
            {
                Vector3 hitPosition = ray.Position + ray.Direction * closestIntersection.Distance;
                Vector3 hitNormal = closestIntersection.HitGeometry->CalculateNormal(ray, hitPosition);
                hitPosition += hitNormal * 0.01f;

                Vector3 lightPower{0.0f};
                lightPower += CalculateDirectionalLightPower(hitPosition, hitNormal);

                Vector3 indirectLight{0.0f};

                if (depth <= 5)
                {
                    float random1 = _random.GetNormalizedFloat();
                    float random2 = _random.GetNormalizedFloat();

                    Vector3 randomHemisphereVector = CosineWeightedSampleHemisphere(random1, random2);
                    Vector3 scatterDirection = TransformFromTangentSpaceToWorldSpace(hitNormal, randomHemisphereVector);

                    indirectLight = CastRayColor(Ray{hitPosition, scatterDirection}, depth + 1);
                }

                const LambertianMaterial* material = closestIntersection.HitGeometry->GetMaterial();

                return material->EmissiveColor + Math::rcp(static_cast<float>(_lights.size() + 1)) * material->Color.ComponentwiseMultiply(lightPower + indirectLight);
            }

            return outputColor;
        }

        Vector3 CalculateDirectionalLightPower(const Vector3& hitPosition, const Vector3& hitNormal) const
        {
            Vector3 lightPower{};

            for (const Light* light : _lights)
            {
                LightRay lightRay = light->CreateShadowRaw(hitPosition, hitNormal);
                float shadowDistance = CastRayDistance(lightRay.ShadowRay);

                lightPower += light->CalculateLightPower(hitPosition, hitNormal, lightRay.PointOnLight, shadowDistance);
            }

            return lightPower;
        }
    };
}