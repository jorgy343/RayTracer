module;

#include "Vcl.h"

export module Scene;

import <cmath>;
import <concepts>;
import <utility>;
import <vector>;

import "Constants.h";

import Alignment;
import Light;
import LambertianMaterial;
import Math;
import MonteCarlo;
import Vector3;
import Random;
import LightRay;
import Geometry;
import IntersectableGeometry;
import Ray;
import Sphere;

namespace RayTracer
{
    export class Scene
    {
    private:
        Vector3 _backgroundColor{0.0f};

        std::vector<const Light*> _lights{};
        std::vector<const IntersectableGeometry*> _geometries{};

        Random _random{};

    public:
        inline constexpr explicit Scene(Vector3 backgroundColor)
            : _backgroundColor{backgroundColor}
        {
            // TODO: This seems to fix some weird module linker bug.
            Sphere sphere{};
        }

        inline constexpr void AddLight(const Light* light)
        {
            _lights.push_back(light);
        }

        inline constexpr void AddGeometry(const IntersectableGeometry* geometry)
        {
            _geometries.push_back(geometry);
        }

        inline constexpr Vector3 CastRayColor(const Ray& ray) const
        {
            return CastRayColor(ray, 1);
        }

    private:
        constexpr float CastRayDistance(const Ray& ray) const
        {
            float closestIntersection = std::numeric_limits<float>::infinity();
            for (const IntersectableGeometry* geometry : _geometries)
            {
                float distance = geometry->IntersectEntrance(ray).Distance;

                if (distance < closestIntersection)
                {
                    closestIntersection = distance;
                }
            }

            return Math::max(0.0f, closestIntersection);
        }

        constexpr Vector3 CastRayColor(const Ray& ray, int depth) const
        {
            IntersectionResult closestIntersection{nullptr, std::numeric_limits<float>::infinity()};
            for (const IntersectableGeometry* geometry : _geometries)
            {
                IntersectionResult intersection = geometry->IntersectEntrance(ray);

                if (intersection.Distance < closestIntersection.Distance)
                {
                    closestIntersection = intersection;
                }
            }

            closestIntersection.Distance = Math::max(0.0f, closestIntersection.Distance);

            Vector3 outputColor = _backgroundColor;

            if (closestIntersection.HitGeometry)
            {
                const LambertianMaterial* material = closestIntersection.HitGeometry->GetMaterial();

                //if (material->EmissiveColor.LengthSquared() > 0.01f)
                //{
                //    return material->Color.ComponentwiseMultiply(material->EmissiveColor);
                //}

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

                return material->EmissiveColor + Math::rcp(static_cast<float>(_lights.size() + 1)) * material->Color.ComponentwiseMultiply(lightPower + indirectLight);
            }

            return outputColor;
        }

        constexpr Vector3 CalculateDirectionalLightPower(const Vector3& hitPosition, const Vector3& hitNormal) const
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