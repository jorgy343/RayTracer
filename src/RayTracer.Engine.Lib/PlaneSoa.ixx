#include <limits>
#include <vector>

#include "Vcl.h"

export module RayTracer.PlaneSoa;

import RayTracer.Alignment;
import RayTracer.IntersectionResult;
import RayTracer.Ray;
import RayTracer.Math;
import RayTracer.Plane;
import RayTracer.GeometrySoa;
import RayTracer.IntersectionResultType;

using namespace vcl;

namespace RayTracer
{
    export template<typename TFloatAllocator = AlignedAllocator<float, 64>, typename TSphereAllocator = AlignedAllocator<const Plane*, 64>>
    class PlaneSoa final : public GeometrySoa<Plane>
    {
    private:
        std::vector<float, TFloatAllocator> _normalX{};
        std::vector<float, TFloatAllocator> _normalY{};
        std::vector<float, TFloatAllocator> _normalZ{};
        std::vector<float, TFloatAllocator> _distance{};
        std::vector<const Plane*, TSphereAllocator> _geometries{};

    public:
        constexpr PlaneSoa(int initialCapacity = 32)
        {
            _normalX.reserve(initialCapacity);
            _normalY.reserve(initialCapacity);
            _normalZ.reserve(initialCapacity);
            _distance.reserve(initialCapacity);
            _geometries.reserve(initialCapacity);
        }

        inline constexpr long long GetCount() const override final
        {
            return _normalX.size();
        }

        constexpr void Add(const Plane* geometry) override final
        {
            _normalX.push_back(geometry->Normal.X);
            _normalY.push_back(geometry->Normal.Y);
            _normalZ.push_back(geometry->Normal.Z);
            _distance.push_back(geometry->Distance);
            _geometries.push_back(geometry);
        }

        constexpr void Finalize() override final
        {
            for (long long i = _normalX.size(); i % 8 != 0; i++)
            {
                _normalX.push_back(std::numeric_limits<float>::infinity());
                _normalY.push_back(std::numeric_limits<float>::infinity());
                _normalZ.push_back(std::numeric_limits<float>::infinity());
                _distance.push_back(std::numeric_limits<float>::infinity());
                _geometries.push_back(nullptr);
            }
        }

        constexpr IntersectionResult IntersectEntrance(const Ray& ray) const override final
        {
            return Intersect<IntersectionResultType::Entrance>(ray);
        }

        constexpr IntersectionResult IntersectExit(const Ray& ray) const override final
        {
            return Intersect<IntersectionResultType::Exit>(ray);
        }

        template <IntersectionResultType TIntersectionResultType>
        inline constexpr IntersectionResult PrivateIntersectSoa(const Ray& ray, int startingGeometryIndex) const
        {
            if (std::is_constant_evaluated())
            {
                float closestDistance = std::numeric_limits<float>::infinity();
                const Plane* closestGeometry = nullptr;

                for (unsigned long long i = startingGeometryIndex; i < _geometries.size() && i < startingGeometryIndex + 8; i++)
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

                Vec8f normalX = Vec8f{}.load_a(&_normalX[startingGeometryIndex]);
                Vec8f normalY = Vec8f{}.load_a(&_normalY[startingGeometryIndex]);
                Vec8f normalZ = Vec8f{}.load_a(&_normalZ[startingGeometryIndex]);

                Vec8f normalDotDirection = SimdDot(normalX, rayDirectionX, normalY, rayDirectionY, normalZ, rayDirectionZ);

                Vec8f rayPositionX{ray.Position.X};
                Vec8f rayPositionY{ray.Position.Y};
                Vec8f rayPositionZ{ray.Position.Z};

                Vec8f normalDotRayPosition = SimdDot(normalX, rayPositionX, normalY, rayPositionY, normalZ, rayPositionZ);

                Vec8f distance = Vec8f{}.load_a(&_distance[startingGeometryIndex]);
                Vec8f entranceDistance = -(distance + normalDotRayPosition) * approx_recipr(normalDotDirection);

                // Make sure infinite8f() is second so nans are replaced with inf.
                Vec8f clampedEntranceDistance = select(entranceDistance >= Vec8f(0.0f), entranceDistance, infinite8f());

                float minimumEntranceDistance = horizontal_min1(clampedEntranceDistance);
                int minimumIndex = horizontal_find_first(Vec8f(minimumEntranceDistance) == clampedEntranceDistance);

                return {
                    _geometries[startingGeometryIndex + (minimumIndex == -1 ? 0 : minimumIndex)],
                    minimumEntranceDistance,
                };
            }
        }

    private:
        template <IntersectionResultType TIntersectionResultType>
        inline constexpr IntersectionResult Intersect(const Ray& ray) const
        {
            IntersectionResult result{nullptr, std::numeric_limits<float>::infinity()};

            for (int i = 0; i + 8 <= _normalX.size(); i += 8)
            {
                IntersectionResult newResult = PrivateIntersectSoa<TIntersectionResultType>(ray, i);

                if (newResult.Distance < result.Distance)
                {
                    result = newResult;
                }
            }

            return result;
        }
    };
}