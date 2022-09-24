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

        const IntersectableGeometry* _rootGeometry{};

    public:
        std::vector<const Light*> Lights{};
        std::vector<const AreaLight*> AreaLights{};

        inline constexpr Scene(const IntersectableGeometry* rootGeometry, Vector3 backgroundColor)
            : _rootGeometry{rootGeometry}, _backgroundColor { backgroundColor }
        {
            // TODO: This seems to fix some weird module linker issue/bug.
            Sphere sphere{};
        }

        inline constexpr void AddLight(const Light* light)
        {
            Lights.push_back(light);
        }

        inline void AddAreaLight(const AreaLight* areaLight)
        {
            AreaLights.push_back(areaLight);
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

            IntersectionResult intersection = _rootGeometry->IntersectEntrance(ray);
            Vector3 outputColor = _backgroundColor;

            if (intersection.HitGeometry)
            {
                const Material* material = intersection.HitGeometry->GetMaterial();

                Vector3 hitPosition = ray.Position + intersection.HitDistance * ray.Direction;
                Vector3 hitNormal = intersection.HitGeometry->CalculateNormal(ray, hitPosition);
                hitPosition += hitNormal * NormalBump;

                outputColor = material->CalculateRenderingEquation(
                    *this,
                    random,
                    depth,
                    intersection.HitGeometry,
                    hitPosition,
                    hitNormal,
                    ray.Direction);
            }

            return outputColor;
        }

        float CastRayDistance(const Ray& ray) const
        {
            IntersectionResult intersection = _rootGeometry->IntersectEntrance(ray);
            return Math::max(0.0f, intersection.HitDistance);
        }
    };
}