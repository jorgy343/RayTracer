#include <limits>
#include <vector>

#include "Vcl.h"

export module RayTracer.ParallelogramSoa;

import RayTracer.Alignment;
import RayTracer.IntersectionResult;
import RayTracer.Ray;
import RayTracer.Math;
import RayTracer.Parallelogram;
import RayTracer.GeometrySoa;
import RayTracer.IntersectionResultType;
import RayTracer.Vec8f3;

using namespace vcl;

namespace RayTracer
{
    export template<typename TFloatAllocator = AlignedAllocator<float, 64>, typename TParallelogramAllocator = AlignedAllocator<const Parallelogram*, 64>>
    class ParallelogramSoa final : public GeometrySoa<Parallelogram>
    {
    private:
        std::vector<float, TFloatAllocator> _positionX{};
        std::vector<float, TFloatAllocator> _positionY{};
        std::vector<float, TFloatAllocator> _positionZ{};

        std::vector<float, TFloatAllocator> _edge1X{};
        std::vector<float, TFloatAllocator> _edge1Y{};
        std::vector<float, TFloatAllocator> _edge1Z{};

        std::vector<float, TFloatAllocator> _edge2X{};
        std::vector<float, TFloatAllocator> _edge2Y{};
        std::vector<float, TFloatAllocator> _edge2Z{};

        std::vector<const Parallelogram*, TParallelogramAllocator> _geometries{};

    public:
        constexpr ParallelogramSoa(int initialCapacity = 32)
        {
            _positionX.reserve(initialCapacity);
            _positionY.reserve(initialCapacity);
            _positionZ.reserve(initialCapacity);

            _edge1X.reserve(initialCapacity);
            _edge1Y.reserve(initialCapacity);
            _edge1Z.reserve(initialCapacity);

            _edge2X.reserve(initialCapacity);
            _edge2Y.reserve(initialCapacity);
            _edge2Z.reserve(initialCapacity);

            _geometries.reserve(initialCapacity);
        }

        inline constexpr long long GetCount() const override final
        {
            return _positionX.size();
        }

        constexpr void Add(const Parallelogram* geometry) override final
        {
            _positionX.push_back(geometry->Position.X);
            _positionY.push_back(geometry->Position.Y);
            _positionZ.push_back(geometry->Position.Z);

            _edge1X.push_back(geometry->Edge1.X);
            _edge1Y.push_back(geometry->Edge1.Y);
            _edge1Z.push_back(geometry->Edge1.Z);

            _edge2X.push_back(geometry->Edge2.X);
            _edge2Y.push_back(geometry->Edge2.Y);
            _edge2Z.push_back(geometry->Edge2.Z);

            _geometries.push_back(geometry);
        }

        constexpr void Finalize() override final
        {
            for (long long i = _positionX.size(); i % 8 != 0; i++)
            {
                _positionX.push_back(std::numeric_limits<float>::infinity());
                _positionY.push_back(std::numeric_limits<float>::infinity());
                _positionZ.push_back(std::numeric_limits<float>::infinity());

                _edge1X.push_back(std::numeric_limits<float>::infinity());
                _edge1Y.push_back(std::numeric_limits<float>::infinity());
                _edge1Z.push_back(std::numeric_limits<float>::infinity());

                _edge2X.push_back(std::numeric_limits<float>::infinity());
                _edge2Y.push_back(std::numeric_limits<float>::infinity());
                _edge2Z.push_back(std::numeric_limits<float>::infinity());

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
                const Parallelogram* closestGeometry = nullptr;

                for (unsigned long long i = startingGeometryIndex; i < _geometries.size() && i < startingGeometryIndex + 8; i++)
                {
                    const Parallelogram* geometry = _geometries[i];

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

                Vec8f edge2X = Vec8f{}.load_a(&_edge2X[startingGeometryIndex]);
                Vec8f edge2Y = Vec8f{}.load_a(&_edge2Y[startingGeometryIndex]);
                Vec8f edge2Z = Vec8f{}.load_a(&_edge2Z[startingGeometryIndex]);

                Vec8f3 p = SimdCrossProduct(rayDirectionX, rayDirectionY, rayDirectionZ, edge2X, edge2Y, edge2Z);

                Vec8f edge1X = Vec8f{}.load_a(&_edge1X[startingGeometryIndex]);
                Vec8f edge1Y = Vec8f{}.load_a(&_edge1Y[startingGeometryIndex]);
                Vec8f edge1Z = Vec8f{}.load_a(&_edge1Z[startingGeometryIndex]);

                Vec8f determinant = SimdDot2(edge1X, edge1Y, edge1Z, p.X, p.Y, p.Z);
                Vec8f invDeterminant = approx_recipr(determinant);

                Vec8f rayPositionX{ray.Position.X};
                Vec8f rayPositionY{ray.Position.Y};
                Vec8f rayPositionZ{ray.Position.Z};

                Vec8f positionX = Vec8f{}.load_a(&_positionX[startingGeometryIndex]);
                Vec8f positionY = Vec8f{}.load_a(&_positionY[startingGeometryIndex]);
                Vec8f positionZ = Vec8f{}.load_a(&_positionZ[startingGeometryIndex]);

                Vec8f tX = rayPositionX - positionX;
                Vec8f tY = rayPositionY - positionY;
                Vec8f tZ = rayPositionZ - positionZ;

                Vec8f a = SimdDot2(tX, tY, tZ, p.X, p.Y, p.Z) * invDeterminant;
                Vec8f3 q = SimdCrossProduct(tX, tY, tZ, edge1X, edge1Y, edge1Z);
                Vec8f b = SimdDot2(rayDirectionX, rayDirectionY, rayDirectionZ, q.X, q.Y, q.Z) * invDeterminant;

                Vec8f entranceDistance = SimdDot2(edge2X, edge2Y, edge2Z, q.X, q.Y, q.Z) * invDeterminant;

                Vec8fb comparison =
                    entranceDistance >= Vec8f(0.0f) &&
                    a >= Vec8f(0.0f) &&
                    a <= Vec8f(1.0f) &&
                    b >= Vec8f(0.0f) &&
                    b <= Vec8f(1.0f);

                // Make sure infinite8f() is second so nans are replaced with inf.
                Vec8f clampedEntranceDistance = select(comparison, entranceDistance, infinite8f());

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

            for (int i = 0; i + 8 <= _positionX.size(); i += 8)
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