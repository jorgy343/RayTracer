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
    private:
        alignas(16) float _normalX[8];
        alignas(16) float _normalY[8];
        alignas(16) float _normalZ[8];
        alignas(16) float _distance[8];
        alignas(16) const Plane* _geometries[8];

    public:
        constexpr PlaneSoa()
        {
            for (int i = 0; i < 8; i++)
            {
                _normalX[i] = std::numeric_limits<float>::infinity();
                _normalY[i] = std::numeric_limits<float>::infinity();
                _normalZ[i] = std::numeric_limits<float>::infinity();
                _distance[i] = std::numeric_limits<float>::infinity();
                _geometries[i] = nullptr;
            }
        }

        constexpr explicit PlaneSoa(std::initializer_list<const Plane*> list)
			: PlaneSoa{}
		{
			size_t index = 0;

			for (auto geometry : list)
			{
				if (index >= 8)
				{
					break;
				}

				Insert(index++, geometry);
			}
		}

        constexpr void Insert(size_t index, const Plane* geometry) override
        {
            assert(index >= 0 && index < 8);

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
            Vec8f rayDirectionX{ray.Direction.X};
            Vec8f rayDirectionY{ray.Direction.Y};
            Vec8f rayDirectionZ{ray.Direction.Z};

            Vec8f normalX = Vec8f{}.load_a(_normalX);
            Vec8f normalY = Vec8f{}.load_a(_normalY);
            Vec8f normalZ = Vec8f{}.load_a(_normalZ);

            Vec8f normalDotDirection = SimdDot(normalX, normalY, normalZ, rayDirectionX, rayDirectionY, rayDirectionZ);

            Vec8f rayPositionX{ray.Position.X};
            Vec8f rayPositionY{ray.Position.Y};
            Vec8f rayPositionZ{ray.Position.Z};

            Vec8f normalDotRayPosition = SimdDot(normalX, normalY, normalZ, rayPositionX, rayPositionY, rayPositionZ);

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
    };
}