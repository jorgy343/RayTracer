module;

#include "Vcl.h"

export module Scene;

import <cmath>;
import <concepts>;
import <utility>;
import <vector>;

import "Constants.h";

import Alignment;
import Geometry;
import IntersectableGeometry;
import LambertianMaterial;
import Light;
import LightRay;
import Math;
import MonteCarlo;
import Random;
import Ray;
import Sphere;
import Vector3;

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
            // TODO: This seems to fix some weird module linker issue/bug.
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
                float distance = geometry->IntersectEntrance(ray).HitDistance;

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

                if (intersection.HitDistance < closestIntersection.HitDistance)
                {
                    closestIntersection = intersection;
                }
            }

            Vector3 outputColor = _backgroundColor;

            if (closestIntersection.HitGeometry)
            {
                const LambertianMaterial* material = closestIntersection.HitGeometry->GetMaterial();

                Vector3 hitPosition = ray.Position + closestIntersection.HitDistance * ray.Direction;
                Vector3 hitNormal = closestIntersection.HitGeometry->CalculateNormal(ray, hitPosition);
                hitPosition += hitNormal * 0.01f;

                //if (material->EmissiveColor.LengthSquared() > 0.01f)
                //{
                //    return material->Color.ComponentwiseMultiply(material->EmissiveColor);
                //}

                Vector3 indirectLightColor{0.0f};

                if (depth <= 5)
                {
                    Vector3 indirectLightDirection = material->CalculateIndirectLightDirection(_random, hitPosition, hitNormal);

                    float indirectLightPdf = material->CalculatePdf(_random, hitPosition, hitNormal, indirectLightDirection);
                    float inverseIndirectLightPdf = indirectLightPdf == 0.0f ? 0.0f : Math::rcp(indirectLightPdf);

                    indirectLightColor = CastRayColor(Ray{hitPosition, indirectLightDirection}, depth + 1) * inverseIndirectLightPdf * Math::max(0.0f, hitNormal * indirectLightDirection);
                }

                // The calculated light power will already include the PDF for the light as well as the cos(theta) term.
                Vector3 lightPower = CalculateDirectionalLightPower(hitPosition, hitNormal);

                return material->EmissiveColor + Math::rcp(static_cast<float>(_lights.size() + 1)) * OneOverPi * material->Color.ComponentwiseMultiply(lightPower + indirectLightColor);
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