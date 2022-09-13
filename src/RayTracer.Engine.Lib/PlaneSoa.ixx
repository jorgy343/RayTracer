module;

#include "Vcl.h"

export module PlaneSoa;

import <limits>;
import <cassert>;

import "Common.h";

import Alignment;
import IntersectionResult;
import Ray;
import Math;
import Plane;
import GeometrySoa;
import IntersectionResultType;

using namespace vcl;

namespace RayTracer
{
    export template <std::size_t Count = 8>
    class alignas(64) PlaneSoa final : public GeometrySoa<Plane>
    {
    private:
        alignas(16) float _normalX[Count];
        alignas(16) float _normalY[Count];
        alignas(16) float _normalZ[Count];
        alignas(16) float _distance[Count];
        alignas(16) const Plane* _geometries[Count];

    public:
        constexpr PlaneSoa()
        {
            for (int i = 0; i < Count; i++)
            {
                _normalX[i] = std::numeric_limits<float>::infinity();
                _normalY[i] = std::numeric_limits<float>::infinity();
                _normalZ[i] = std::numeric_limits<float>::infinity();
                _distance[i] = std::numeric_limits<float>::infinity();
                _geometries[i] = nullptr;
            }
        }

        constexpr void Insert(int index, const Plane* geometry) override final
        {
            assert(index >= 0 && index < Count);

            _normalX[index] = geometry->Normal.X;
            _normalY[index] = geometry->Normal.Y;
            _normalZ[index] = geometry->Normal.Z;
            _distance[index] = geometry->Distance;
            _geometries[index] = geometry;
        }

        constexpr IntersectionResult IntersectEntrance(const Ray& ray) const override final
        {
            return Intersect<IntersectionResultType::Entrance>(ray);
        }

        constexpr IntersectionResult IntersectExit(const Ray& ray) const override final
        {
            return Intersect<IntersectionResultType::Exit>(ray);
        }

    private:
        template <IntersectionResultType TIntersectionResultType>
        force_inline constexpr IntersectionResult Intersect(const Ray& ray) const
        {
            if (std::is_constant_evaluated() || Count != 8)
            {
                float closestDistance = std::numeric_limits<float>::infinity();
                const Plane* closestGeometry = nullptr;

                for (int i = 0; i < Count; i++)
                {
                    const Plane* geometry = _geometries[i];

                    if (geometry == nullptr)
                    {
                        continue;
                    }

                    float distance = geometry->Intersect<TIntersectionResultType>(ray);

                    if (distance < closestDistance)
                    {
                        closestDistance = distance;
                        closestGeometry = geometry;
                    }
                }

                return IntersectionResult{closestGeometry, closestDistance};
            }
            else
            {
                Vec8f rayDirectionX{ray.Direction.X};
                Vec8f rayDirectionY{ray.Direction.Y};
                Vec8f rayDirectionZ{ray.Direction.Z};

                Vec8f normalX = Vec8f{}.load_a(_normalX);
                Vec8f normalY = Vec8f{}.load_a(_normalY);
                Vec8f normalZ = Vec8f{}.load_a(_normalZ);

                Vec8f normalDotDirection = SimdDot(normalX, rayDirectionX, normalY, rayDirectionY, normalZ, rayDirectionZ);

                Vec8f rayPositionX{ray.Position.X};
                Vec8f rayPositionY{ray.Position.Y};
                Vec8f rayPositionZ{ray.Position.Z};

                Vec8f normalDotRayPosition = SimdDot(normalX, rayPositionX, normalY, rayPositionY, normalZ, rayPositionZ);

                Vec8f distance = Vec8f{}.load_a(_distance);
                Vec8f entranceDistance = -(distance + normalDotRayPosition) * approx_recipr(normalDotDirection);

                // Make sure infinite8f() is second so nans are replaced with inf.
                Vec8f clampedEntranceDistance = select(entranceDistance >= Vec8f(0.0f), entranceDistance, infinite8f());

                float minimumEntranceDistance = horizontal_min1(clampedEntranceDistance);
                int minimumIndex = horizontal_find_first(Vec8f(minimumEntranceDistance) == clampedEntranceDistance);

                return {
                    _geometries[minimumIndex == -1 ? 0 : minimumIndex],
                    minimumEntranceDistance,
                };
            }
        }
    };
}