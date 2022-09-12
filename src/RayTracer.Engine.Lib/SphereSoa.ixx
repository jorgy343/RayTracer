#include <cmath>
#include <vector>

#include "Vcl.h"

export module RayTracer.SphereSoa;

import RayTracer.Alignment;
import RayTracer.IntersectionResult;
import RayTracer.Ray;
import RayTracer.Math;
import RayTracer.Sphere;
import RayTracer.GeometrySoa;
import RayTracer.IntersectionResultType;

using namespace vcl;

namespace RayTracer
{
    export template<typename TFloatAllocator = AlignedAllocator<float, 64>, typename TSphereAllocator = AlignedAllocator<const Sphere*, 64>>
    class SphereSoa final : public GeometrySoa<Sphere>
    {
    private:
        std::vector<float, TFloatAllocator> _positionX{};
        std::vector<float, TFloatAllocator> _positionY{};
        std::vector<float, TFloatAllocator> _positionZ{};
        std::vector<float, TFloatAllocator> _radius{};
        std::vector<const Sphere*, TSphereAllocator> _geometries{};
        
    public:
        constexpr SphereSoa(int initialCapacity = 32)
        {
            _positionX.reserve(initialCapacity);
            _positionY.reserve(initialCapacity);
            _positionZ.reserve(initialCapacity);
            _radius.reserve(initialCapacity);
            _geometries.reserve(initialCapacity);
        }

        inline constexpr long long GetCount() const override final
        {
            return _positionX.size();
        }

        constexpr void Add(const Sphere* geometry) override final
        {
            _positionX.push_back(geometry->Position.X);
            _positionY.push_back(geometry->Position.Y);
            _positionZ.push_back(geometry->Position.Z);
            _radius.push_back(geometry->Radius);
            _geometries.push_back(geometry);
        }

        constexpr void Finalize() override final
        {
            for (long long i = _positionX.size(); i % 8 != 0; i++)
            {
                _positionX.push_back(std::numeric_limits<float>::infinity());
                _positionY.push_back(std::numeric_limits<float>::infinity());
                _positionZ.push_back(std::numeric_limits<float>::infinity());
                _radius.push_back(std::numeric_limits<float>::infinity());
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
                const Sphere* closestGeometry = nullptr;

                for (unsigned long long i = startingGeometryIndex; i < _geometries.size() && i < startingGeometryIndex + 8; i++)
                {
                    const Sphere* geometry = _geometries[i];

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
                Vec8f rayPositionX{ray.Position.X};
                Vec8f rayPositionY{ray.Position.Y};
                Vec8f rayPositionZ{ray.Position.Z};

                Vec8f spherePositionX = Vec8f{}.load_a(&_positionX[startingGeometryIndex]);
                Vec8f spherePositionY = Vec8f{}.load_a(&_positionY[startingGeometryIndex]);
                Vec8f spherePositionZ = Vec8f{}.load_a(&_positionZ[startingGeometryIndex]);

                Vec8f vX = rayPositionX - spherePositionX;
                Vec8f vY = rayPositionY - spherePositionY;
                Vec8f vZ = rayPositionZ - spherePositionZ;

                Vec8f rayDirectionX{ray.Direction.X};
                Vec8f rayDirectionY{ray.Direction.Y};
                Vec8f rayDirectionZ{ray.Direction.Z};

                Vec8f sphereRadius = Vec8f{}.load_a(&_radius[startingGeometryIndex]);

                Vec8f a = SimdDot(rayDirectionX, rayDirectionX, rayDirectionY, rayDirectionY, rayDirectionZ, rayDirectionZ);
                Vec8f b = SimdDot(vX, rayDirectionX, vY, rayDirectionY, vZ, rayDirectionZ);
                Vec8f c = SimdDot(vX, vX, vY, vY, vZ, vZ) - (sphereRadius * sphereRadius);

                Vec8f discriminant = (b * b) - (a * c);
                Vec8f discriminantSqrt = sqrt(discriminant);

                Vec8f reciprocalA = approx_recipr(a);
                Vec8f negativeB = -b;

                Vec8f exitDistance = (negativeB + discriminantSqrt) * reciprocalA;

                Vec8f result;
                if constexpr (TIntersectionResultType == IntersectionResultType::Entrance)
                {
                    Vec8f entranceDistance = (negativeB - discriminantSqrt) * reciprocalA;
                    result = entranceDistance;
                }
                else
                {
                    result = exitDistance;
                }

                // Make sure infinite8f() is second so nans are replaced with inf.
                Vec8f clampedResult = select(exitDistance >= Vec8f(0.0f), result, infinite8f());

                float minimumEntranceDistance = horizontal_min1(clampedResult);
                int minimumIndex = horizontal_find_first(Vec8f(minimumEntranceDistance) == clampedResult);

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

    consteval float Test()
    {
        SphereSoa<std::allocator<float>, std::allocator<const Sphere*>> soa{};

        Sphere sphere1{{0, 0, 0}, 5, nullptr};
        soa.Add(&sphere1);

        soa.Finalize();

        Ray ray{{0, 0, -10}, {0, 0, 1}};
        IntersectionResult result = soa.IntersectEntrance(ray);

        return result.Distance;
    }

    static_assert(Test() == 5.0f);
}