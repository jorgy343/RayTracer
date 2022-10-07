module;

#include "Vcl.h"

export module PlaneSoa;

import <cassert>;
import <initializer_list>;
import <limits>;

import "Common.h";

import Alignment;
import GeometrySoa;
import IntersectionResult;
import IntersectionResultType;
import Math;
import Plane;
import Ray;

using namespace vcl;

namespace Yart
{
    export class __declspec(dllexport) alignas(64) PlaneSoa : public GeometrySoa<Plane>
    {
    public:
        static constexpr size_t Size = std::same_as<real, float> ? 8 : 4;

    private:
        using VclVec = typename std::conditional<std::same_as<real, float>, Vec8f, Vec4d>::type;

        alignas(sizeof(real) * 4) real _normalX[Size];
        alignas(sizeof(real) * 4) real _normalY[Size];
        alignas(sizeof(real) * 4) real _normalZ[Size];
        alignas(sizeof(real) * 4) real _distance[Size];

        alignas(sizeof(real) * 4) const Plane* _geometries[Size];

    public:
        constexpr PlaneSoa()
        {
            for (int i = 0; i < Size; i++)
            {
                _normalX[i] = std::numeric_limits<real>::infinity();
                _normalY[i] = std::numeric_limits<real>::infinity();
                _normalZ[i] = std::numeric_limits<real>::infinity();
                _distance[i] = std::numeric_limits<real>::infinity();

                _geometries[i] = nullptr;
            }
        }

        constexpr explicit PlaneSoa(std::initializer_list<const Plane*> list)
            : PlaneSoa{}
        {
            size_t index = 0;

            for (auto geometry : list)
            {
                if (index >= Size)
                {
                    break;
                }

                Insert(index++, geometry);
            }
        }

        constexpr void Insert(size_t index, const Plane* geometry) override
        {
            assert(index >= 0 && index < Size);

            _normalX[index] = geometry->Normal.X;
            _normalY[index] = geometry->Normal.Y;
            _normalZ[index] = geometry->Normal.Z;
            _distance[index] = geometry->Distance;

            _geometries[index] = geometry;
        }

        IntersectionResult IntersectEntrance(const Ray& ray) const override
        {
            return Intersect(ray);
        }

        IntersectionResult IntersectExit(const Ray& ray) const override
        {
            return Intersect(ray);
        }

    private:
        force_inline IntersectionResult Intersect(const Ray& ray) const
        {
            VclVec rayDirectionX{ray.Direction.X};
            VclVec rayDirectionY{ray.Direction.Y};
            VclVec rayDirectionZ{ray.Direction.Z};

            VclVec normalX = VclVec{}.load_a(_normalX);
            VclVec normalY = VclVec{}.load_a(_normalY);
            VclVec normalZ = VclVec{}.load_a(_normalZ);

            VclVec normalDotDirection = SimdDot(normalX, normalY, normalZ, rayDirectionX, rayDirectionY, rayDirectionZ);

            VclVec rayPositionX{ray.Position.X};
            VclVec rayPositionY{ray.Position.Y};
            VclVec rayPositionZ{ray.Position.Z};

            VclVec normalDotRayPosition = SimdDot(normalX, normalY, normalZ, rayPositionX, rayPositionY, rayPositionZ);

            VclVec distance = VclVec{}.load_a(_distance);
            VclVec entranceDistance = -(distance + normalDotRayPosition) * approx_recipr(normalDotDirection);

            // Make sure infinite8f() is second so nans are replaced with inf.
            VclVec clampedEntranceDistance = select(entranceDistance >= VclVec{real{0.0}}, entranceDistance, VclVec{std::numeric_limits<real>::infinity()});

            real minimumEntranceDistance = horizontal_min1(clampedEntranceDistance);
            int minimumIndex = horizontal_find_first(VclVec(minimumEntranceDistance) == clampedEntranceDistance);

            return {
                _geometries[minimumIndex == -1 ? 0 : minimumIndex],
                minimumEntranceDistance,
            };
        }
    };
}