module;

#include "Vcl.h"

export module Scene;

import <cmath>;
import <concepts>;
import <memory>;
import <utility>;
import <vector>;

import "Constants.h";

import Alignment;
import AreaLight;
import Geometry;
import IntersectableGeometry;
import Light;
import Material;
import Math;
import Random;
import Ray;
import Sphere;
import Vector3;

namespace Yart
{
    export class Scene
    {
    private:
        Vector3 _backgroundColor{0.0f};

        std::vector<std::shared_ptr<const IntersectableGeometry>> _geometries{};

    public:
        std::vector<const Light*> Lights{};
        std::vector<std::shared_ptr<const AreaLight>> AreaLights{};

        inline constexpr explicit Scene(Vector3 backgroundColor)
            : _backgroundColor{backgroundColor}
        {
            // TODO: This seems to fix some weird module linker issue/bug.
            Sphere sphere{};
        }

        inline constexpr void AddLight(const Light* light)
        {
            Lights.push_back(light);
        }

        inline void AddAreaLight(std::shared_ptr<const AreaLight> areaLight)
        {
            AreaLights.push_back(areaLight);
        }

        inline void AddGeometry(std::shared_ptr<const IntersectableGeometry> geometry)
        {
            _geometries.push_back(geometry);
        }

        inline Vector3 CastRayColor(const Ray& ray, const Random& random) const
        {
            return CastRayColor(ray, 1, random);
        }

        Vector3 CastRayColor(const Ray& ray, int depth, const Random& random) const
        {
            if (depth > 7)
            {
                return Vector3{};
            }

            IntersectionResult closestIntersection{nullptr, std::numeric_limits<float>::infinity()};
            for (const auto& geometry : _geometries)
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

                Vector3 hitPosition = ray.Position + closestIntersection.HitDistance * ray.Direction;
                Vector3 hitNormal = closestIntersection.HitGeometry->CalculateNormal(ray, hitPosition);
                hitPosition += hitNormal * NormalBump;

                outputColor = material->CalculateRenderingEquation(*this, random, depth, hitPosition, hitNormal, ray.Direction);
            }

            return outputColor;
        }

        float CastRayDistance(const Ray& ray) const
        {
            float closestIntersection = std::numeric_limits<float>::infinity();
            for (auto geometry : _geometries)
            {
                float distance = geometry->IntersectEntrance(ray).HitDistance;

                if (distance < closestIntersection)
                {
                    closestIntersection = distance;
                }
            }

            return Math::max(0.0f, closestIntersection);
        }
    };
}