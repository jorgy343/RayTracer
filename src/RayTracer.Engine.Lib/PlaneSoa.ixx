#include <cmath>

#include "Vcl.h"

export module RayTracer.PlaneSoa;

import RayTracer.Alignment;
import RayTracer.IntersectionResult;
import RayTracer.Ray;
import RayTracer.Math;
import RayTracer.Plane;
import RayTracer.GeometrySoa;

using namespace vcl;

namespace RayTracer
{
    export class PlaneSoa final : public GeometrySoa<Plane>
    {
    private:
        AlignedVector<float, 64> _normalX{};
        AlignedVector<float, 64> _normalY{};
        AlignedVector<float, 64> _normalZ{};
        AlignedVector<float, 64> _distance{};
        AlignedVector<const Plane*, 64> _planes{};

    public:
        PlaneSoa(int initialCapacity = 32)
        {
            _normalX.reserve(initialCapacity);
            _normalY.reserve(initialCapacity);
            _normalZ.reserve(initialCapacity);
            _distance.reserve(initialCapacity);
            _planes.reserve(initialCapacity);
        }

        char GetCount() const override final
        {
            return static_cast<char>(_normalX.size());
        }

        void Add(const Plane* plane) override final
        {
            _normalX.push_back(plane->Normal.X);
            _normalY.push_back(plane->Normal.Y);
            _normalZ.push_back(plane->Normal.Z);
            _distance.push_back(plane->Distance);
            _planes.push_back(plane);
        }

        void Finalize() override final
        {
            for (auto i = _normalX.size(); i % 8 != 0; i++)
            {
                _normalX.push_back(std::numeric_limits<float>::infinity());
                _normalY.push_back(std::numeric_limits<float>::infinity());
                _normalZ.push_back(std::numeric_limits<float>::infinity());
                _distance.push_back(std::numeric_limits<float>::infinity());
                _planes.push_back(nullptr);
            }
        }

        IntersectionResult<Plane> Intersect(const Ray& ray) const override final
        {
            IntersectionResult<Plane> result{nullptr, std::numeric_limits<float>::infinity()};

            for (int i = 0; i + 8 <= _normalX.size(); i += 8)
            {
                IntersectionResult<Plane> newResult = PrivateIntersectSoa(ray, i);

                if (newResult.Distance < result.Distance)
                {
                    result = newResult;
                }
            }

            return result;
        }

        inline IntersectionResult<Plane> PrivateIntersectSoa(const Ray& ray, int startingGeometryIndex) const override final
        {
            Vec8f rayDirectionX{ray.Direction.X};
            Vec8f rayDirectionY{ray.Direction.Y};
            Vec8f rayDirectionZ{ray.Direction.Z};

            Vec8f normalX = Vec8f{}.load_a(&_normalX[startingGeometryIndex]);
            Vec8f normalY = Vec8f{}.load_a(&_normalY[startingGeometryIndex]);
            Vec8f normalZ = Vec8f{}.load_a(&_normalZ[startingGeometryIndex]);
            
            Vec8f normalDotDirection = SimdDot(normalX, rayDirectionX, normalY, rayDirectionY, normalZ, rayDirectionZ);

            // Normally we would check if the dot product was normal, but let's just assume that never happens.

            Vec8f rayPositionX{ray.Position.X};
            Vec8f rayPositionY{ray.Position.Y};
            Vec8f rayPositionZ{ray.Position.Z};

            Vec8f normalDotRayPosition = SimdDot(normalX, rayPositionX, normalY, rayPositionY, normalZ, rayPositionZ);

            Vec8f distance = Vec8f{}.load_a(&_distance[startingGeometryIndex]);
            Vec8f entranceDistance = -(distance + normalDotRayPosition) * approx_recipr(normalDotDirection);

            // Make sure infinite8f() is second so nans are replaced with inf.
            Vec8f clampedEntranceDistance = select(entranceDistance >= Vec8f(0.0f), entranceDistance, infinite8f());

            float minimumEntranceDistance = horizontal_min(clampedEntranceDistance);
            int minimumIndex = horizontal_find_first(Vec8f(minimumEntranceDistance) == clampedEntranceDistance);

            return {
                _planes[startingGeometryIndex + (minimumIndex == -1 ? 0 : minimumIndex)],
                minimumEntranceDistance,
            };
        }
    };
}