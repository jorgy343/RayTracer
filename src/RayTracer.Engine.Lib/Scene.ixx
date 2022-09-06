#include <cmath>

#include "Constants.h"

export module RayTracer.Scene;

import RayTracer.Alignment;
import RayTracer.DirectionalLight;
import RayTracer.LambertianMaterial;
import RayTracer.Math;
import RayTracer.MonteCarlo;
import RayTracer.Sphere;
import RayTracer.SphereSoa;
import RayTracer.Plane;
import RayTracer.PlaneSoa;
import RayTracer.AxisAlignedBox;
import RayTracer.AxisAlignedBoxSoa;
import RayTracer.Geometry;
import RayTracer.Vector3;

namespace RayTracer
{
    export class Scene
    {
    private:
        Vector3 _backgroundColor{0.0f};

        AlignedVector<const DirectionalLight*, 64> _directionalLights{};

        SphereSoa _sphereSoa{};
        PlaneSoa _planeSoa{};
        AxisAlignedBoxSoa _axisAlignedBoxSoa{};

    public:
        Scene(Vector3 backgroundColor)
            : _backgroundColor{backgroundColor}
        {
            
        }

        void AddDirectionalLight(const DirectionalLight* directionalLight)
        {
            _directionalLights.push_back(directionalLight);
        }

        void AddSphere(const Sphere* sphere)
        {
            _sphereSoa.Add(sphere);
        }

        void AddPlane(const Plane* plane)
        {
            _planeSoa.Add(plane);
        }

        void AddAxisAlignedBox(const AxisAlignedBox* axisAlignedBox)
        {
            _axisAlignedBoxSoa.Add(axisAlignedBox);
        }

        void Finalize()
        {
            _sphereSoa.Finalize();
            _planeSoa.Finalize();
            _axisAlignedBoxSoa.Finalize();
        }

        Vector3 CastRayColor(const Ray& ray) const
        {
            return CastRayColor(ray, 1);
        }

    private:
        float CastRayDistance(const Ray& ray) const
        {
            IntersectionResult<Sphere> sphereIntersectionResult = _sphereSoa.Intersect(ray);
            IntersectionResult<Geometry> planeIntersectionResult = _planeSoa.Intersect(ray);
            IntersectionResult<Geometry> axisAlignedBoxIntersectionResult = _axisAlignedBoxSoa.Intersect(ray);

            return FastMax(0.0f, FastMin(FastMin(sphereIntersectionResult.Distance, planeIntersectionResult.Distance), axisAlignedBoxIntersectionResult.Distance));
        }

        Vector3 CastRayColor(const Ray& ray, int depth) const
        {
            IntersectionResult<Geometry> sphereIntersectionResult = _sphereSoa.Intersect(ray);
            IntersectionResult<Geometry> planeIntersectionResult = _planeSoa.Intersect(ray);
            IntersectionResult<Geometry> axisAlignedBoxIntersectionResult = _axisAlignedBoxSoa.Intersect(ray);

            IntersectionResult<Geometry> intersectionResult = sphereIntersectionResult.Distance < planeIntersectionResult.Distance ? sphereIntersectionResult : planeIntersectionResult;
            intersectionResult = intersectionResult.Distance < axisAlignedBoxIntersectionResult.Distance ? intersectionResult : axisAlignedBoxIntersectionResult;

            intersectionResult.Distance = FastMax(0.0f, intersectionResult.Distance);

            Vector3 outputColor = _backgroundColor;

            if (intersectionResult.HitGeometry)
            {
                Vector3 hitPosition = ray.Position + ray.Direction * intersectionResult.Distance;
                Vector3 hitNormal = intersectionResult.HitGeometry->CalculateNormal(ray, hitPosition);
                hitPosition += hitNormal * 0.01f;

                Vector3 lightPower{0.0f};
                lightPower += CalculateDirectionalLightPower(hitPosition, hitNormal);

                Vector3 indirectLight{0.0f};

                if (depth <= 5)
                {
                    float random1 = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
                    float random2 = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);

                    Vector3 randomHemisphereVector = CosineWeightedSampleHemisphere(random1, random2);
                    Vector3 scatterDirection = TransformFromTangentSpaceToWorldSpace(hitNormal, randomHemisphereVector);

                    indirectLight = CastRayColor(Ray{hitPosition, scatterDirection}, depth + 1);
                }

                const LambertianMaterial* material = intersectionResult.HitGeometry->GetMaterial();

                return material->EmissiveColor + FastReciprical(_directionalLights.size() + 1) * material->Color.ComponentwiseMultiply(lightPower + indirectLight);
            }

            return outputColor;
        }

        Vector3 CalculateDirectionalLightPower(const Vector3& hitPosition, const Vector3& hitNormal) const
        {
            Vector3 lightPower{0.0f};

            for (const DirectionalLight* light : _directionalLights)
            {
                Ray shadowRay{hitPosition, light->ReversedDirection};
                float shadowDistance = CastRayDistance(shadowRay);

                if (shadowDistance == std::numeric_limits<float>::infinity())
                {
                    lightPower += light->Color * FastMax(0.0f, hitNormal * light->ReversedDirection);
                }
            }

            return lightPower;
        }
    };
}