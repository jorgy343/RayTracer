#include <cmath>

export module RayTracer.Scene;

import RayTracer.Alignment;
import RayTracer.DirectionalLight;
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

        float CastRayDistance(const Ray& ray)
        {
            IntersectionResult<Sphere> intersectionResult = _sphereSoa.Intersect(ray);

            return intersectionResult.Distance;
        }

        Vector3 CastRay(const Ray& ray)
        {
            IntersectionResult<Sphere> intersectionResult = _sphereSoa.Intersect(ray);

            Vector3 outputColor = _backgroundColor;

            if (intersectionResult.Shape)
            {
                Vector3 hitPosition = ray.Position + ray.Direction * intersectionResult.Distance;
                Vector3 hitNormal = intersectionResult.Shape->CalculateNormal(hitPosition);

                for (const DirectionalLight* light : _directionalLights)
                {
                    Ray shadowRay{hitPosition, -light->Direction};

                    float shadowResult = CastRayDistance(shadowRay);

                    if (!isinf(shadowResult))
                    {

                    }
                }
            }

            return outputColor;
        }
    };
}