#include <cmath>

#include "Constants.h"

export module RayTracer.Scene;

import RayTracer.Alignment;
import RayTracer.DirectionalLight;
import RayTracer.LambertianMaterial;
import RayTracer.Math;
import RayTracer.SphereSoa;
import RayTracer.Vector3;

namespace RayTracer
{
    export class Scene
    {
    private:
        Vector3 _backgroundColor{0.0f};

        AlignedVector<const DirectionalLight*, 64> _directionalLights{};

        SphereSoa _sphereSoa{};

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
            _sphereSoa.AddSphere(sphere);
        }

        void Finalize()
        {
            _sphereSoa.Finalize();
        }

        Vector3 CastRayColor(const Ray& ray) const
        {
            return CastRayColor(ray, 1);
        }

    private:
        float CastRayDistance(const Ray& ray) const
        {
            IntersectionResult<Sphere> intersectionResult = _sphereSoa.Intersect(ray);

            return intersectionResult.Distance;
        }

        Vector3 CastRayColor(const Ray& ray, int depth) const
        {
            IntersectionResult<Sphere> intersectionResult = _sphereSoa.Intersect(ray);
            intersectionResult.Distance -= 0.01f; // Bump the closest intersection backwards to ensure we don't shoot rays from inside the shape.

            Vector3 outputColor = _backgroundColor;

            if (intersectionResult.Shape)
            {
                Vector3 hitPosition = ray.Position + ray.Direction * intersectionResult.Distance;
                Vector3 hitNormal = intersectionResult.Shape->CalculateNormal(hitPosition);

                Vector3 lightPower{0.0f};
                lightPower += CalculateDirectionalLightPower(hitPosition, hitNormal);

                Vector3 indirectLight{0.0f};

                Ray indirectLightingRay;

                if (depth <= 5)
                {
                    //CastRayColor(indirectLightingRay, depth + 1);
                }

                const LambertianMaterial* material = intersectionResult.Shape->Material;

                return material->EmissiveColor + ((lightPower * OneOverPi) + (indirectLight * OneOverPi)) * material->Color;
            }

            return outputColor;
        }

        Vector3 CalculateDirectionalLightPower(const Vector3& hitPosition, const Vector3& hitNormal) const
        {
            Vector3 lightPower{0.0f};

            for (const DirectionalLight* light : _directionalLights)
            {
                Vector3 reversedLightDirection = -light->Direction; // TODO: Maybe store reversed direction on light.

                Ray shadowRay{hitPosition, reversedLightDirection};
                float shadowDistance = CastRayDistance(shadowRay);

                if (shadowDistance == std::numeric_limits<float>::infinity())
                {
                    lightPower += light->Color * FastMax(0.0f, hitNormal * reversedLightDirection);
                }
            }

            return lightPower;
        }
    };
}