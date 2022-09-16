module;

#include "Vcl.h"

export module Scene;

import <cmath>;
import <concepts>;
import <utility>;
import <vector>;

import "Constants.h";

import Alignment;
import AreaLight;
import Geometry;
import IntersectableGeometry;
import LambertianMaterial;
import Material;
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

        std::vector<const AreaLight*> _lights{};
        std::vector<const IntersectableGeometry*> _geometries{};

        Random _random{};

    public:
        inline constexpr explicit Scene(Vector3 backgroundColor)
            : _backgroundColor{backgroundColor}
        {
            // TODO: This seems to fix some weird module linker issue/bug.
            Sphere sphere{};
        }

        inline constexpr void AddLight(const AreaLight* light)
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
            if (depth > 5)
            {
                return Vector3{};
            }

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
                const Material* material = closestIntersection.HitGeometry->GetMaterial();

                if (material->EmissiveColor.LengthSquared() > 0.01f)
                {
                    return material->EmissiveColor;
                }

                Vector3 hitPosition = ray.Position + closestIntersection.HitDistance * ray.Direction;
                Vector3 hitNormal = closestIntersection.HitGeometry->CalculateNormal(ray, hitPosition);
                hitPosition += hitNormal * 0.01f;








                int indexOfLightToSample = _random.GetInteger() % _lights.size();
                const AreaLight* light = _lights[indexOfLightToSample];

                Vector3 outgoingDirection;

                float whereToShootRay = _random.GetNormalizedFloat();
                if (whereToShootRay > 0.5f)
                {
                    // Indirect light sample according to material.
                    outgoingDirection = material->GenerateRandomDirection(_random, hitPosition, hitNormal, ray.Direction);
                }
                else
                {
                    // Direct light sample to a random light.
                    outgoingDirection = light->GenerateRandomDirectionTowardsLight(_random, hitPosition, hitNormal);
                }

                float materialInversePdf = material->CalculateInversePdf(_random, hitPosition, hitNormal, outgoingDirection);
                float lightInversePdf = light->CalculateInversePdf(_random, hitPosition, hitNormal, ray.Direction, outgoingDirection);

                float inversePdf = 0.5f * lightInversePdf * _lights.size() + 0.5f * materialInversePdf;

                Ray outgoingRay = Ray{hitPosition, outgoingDirection};
                Vector3 indirectColorSample = CastRayColor(outgoingRay, depth + 1);

                float brdf = material->CalculateBrdf(_random, hitPosition, hitNormal, outgoingDirection);
                float cosineTheta = Math::max(0.0f, hitNormal * outgoingDirection);

                outputColor = brdf * material->Color.ComponentwiseMultiply(indirectColorSample) * inversePdf * cosineTheta;








                //Vector3 outgoingDirection;
                //float inversePdf;

                //float whereToShootRay = _random.GetNormalizedFloat();
                //if (whereToShootRay > 0.5f)
                //{
                //    // Indirect light sample according to material.
                //    ScatterResult scatterResult = material->CalculateScatterData(_random, hitPosition, hitNormal, ray.Direction);

                //    outgoingDirection = scatterResult.OutgoingDirection;
                //    inversePdf = scatterResult.InversePdf;
                //}
                //else
                //{
                //    // Direct light sample to a random light.
                //    int indexOfLightToSample = _random.GetInteger() % _lights.size();
                //    const AreaLight* light = _lights[indexOfLightToSample];

                //    outgoingDirection = light->GenerateRandomDirectionTowardsLight(_random, hitPosition, hitNormal);
                //    inversePdf = light->CalculateInversePdf(_random, hitPosition, hitNormal, ray.Direction, outgoingDirection) * _lights.size();
                //}

                //Ray outgoingRay = Ray{hitPosition, outgoingDirection};
                //Vector3 indirectColorSample = CastRayColor(outgoingRay, depth + 1);

                //float brdf = material->CalculateBrdf(_random, hitPosition, hitNormal, outgoingDirection);
                //float cosineTheta = Math::max(0.0f, hitNormal * outgoingDirection);

                //outputColor = brdf * material->Color.ComponentwiseMultiply(indirectColorSample) * inversePdf * cosineTheta;
            }

            return outputColor;
        }
    };
}