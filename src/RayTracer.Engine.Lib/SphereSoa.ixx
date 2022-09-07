#include <cmath>

#include "Vcl.h"

export module RayTracer.SphereSoa;

import RayTracer.Alignment;
import RayTracer.IntersectionResult;
import RayTracer.Ray;
import RayTracer.Math;
import RayTracer.Sphere;
import RayTracer.GeometrySoa;

using namespace vcl;

namespace RayTracer
{
    export class SphereSoa final : public GeometrySoa<Sphere>
    {
    private:
        AlignedVector<float, 64> _positionX{};
        AlignedVector<float, 64> _positionY{};
        AlignedVector<float, 64> _positionZ{};
        AlignedVector<float, 64> _radius{};
        AlignedVector<const Sphere*, 64> _geometries{};
        
    public:
        SphereSoa(int initialCapacity = 32)
        {
            _positionX.reserve(initialCapacity);
            _positionY.reserve(initialCapacity);
            _positionZ.reserve(initialCapacity);
            _radius.reserve(initialCapacity);
            _geometries.reserve(initialCapacity);
        }

        char GetCount() const override final
        {
            return static_cast<char>(_positionX.size());
        }

        void Add(const Sphere* geometry) override final
        {
            _positionX.push_back(geometry->Position.X);
            _positionY.push_back(geometry->Position.Y);
            _positionZ.push_back(geometry->Position.Z);
            _radius.push_back(geometry->Radius);
            _geometries.push_back(geometry);
        }

        void Finalize() override final
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

        IntersectionResult<Sphere> Intersect(const Ray& ray) const override final
        {
            IntersectionResult<Sphere> result{nullptr, std::numeric_limits<float>::infinity()};

            for (int i = 0; i + 8 <= _positionX.size(); i += 8)
            {
                IntersectionResult<Sphere> newResult = PrivateIntersectSoa(ray, i);

                if (newResult.Distance < result.Distance)
                {
                    result = newResult;
                }
            }

            return result;
        }

        inline IntersectionResult<Sphere> PrivateIntersectSoa(const Ray& ray, int startingGeometryIndex) const override final
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
            Vec8f entranceDistance = (negativeB - discriminantSqrt) * reciprocalA;

            // Make sure infinite8f() is second so nans are replaced with inf.
            Vec8f clampedEntranceDistance = select(exitDistance >= Vec8f(0.0f), entranceDistance, infinite8f());

            float minimumEntranceDistance = horizontal_min1(clampedEntranceDistance);
            int minimumIndex = horizontal_find_first(Vec8f(minimumEntranceDistance) == clampedEntranceDistance);

            return {
                _geometries[startingGeometryIndex + (minimumIndex == -1 ? 0 : minimumIndex)],
                minimumEntranceDistance,
            };
        }
    };
}