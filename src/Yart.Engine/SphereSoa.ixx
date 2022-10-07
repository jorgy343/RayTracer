module;

#include "Vcl.h"

export module SphereSoa;

import <cassert>;
import <cmath>;
import <initializer_list>;

import "Common.h";

import Alignment;
import BoundingBox;
import GeometrySoa;
import IntersectionResult;
import IntersectionResultType;
import Math;
import Ray;
import Sphere;

using namespace vcl;

namespace Yart
{
    export class __declspec(dllexport) alignas(64) SphereSoa : public GeometrySoa<Sphere>
    {
    public:
        static constexpr size_t Size = std::same_as<real, float> ? 8 : 4;

    private:
        using VclVec = typename std::conditional<std::same_as<real, float>, Vec8f, Vec4d>::type;

        alignas(sizeof(real) * 4) real _positionX[Size];
        alignas(sizeof(real) * 4) real _positionY[Size];
        alignas(sizeof(real) * 4) real _positionZ[Size];
        alignas(sizeof(real) * 4) real _radius[Size];

        alignas(16) const Sphere* _geometries[Size];

    public:
        constexpr SphereSoa()
        {
            for (int i = 0; i < Size; i++)
            {
                _positionX[i] = std::numeric_limits<real>::infinity();
                _positionY[i] = std::numeric_limits<real>::infinity();
                _positionZ[i] = std::numeric_limits<real>::infinity();
                _radius[i] = std::numeric_limits<real>::infinity();

                _geometries[i] = nullptr;
            }
        }

        constexpr explicit SphereSoa(std::initializer_list<const Sphere*> list)
            : SphereSoa{}
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

        constexpr void Insert(size_t index, const Sphere* geometry) override
        {
            assert(index >= 0 && index < Size);

            _positionX[index] = geometry->Position.X;
            _positionY[index] = geometry->Position.Y;
            _positionZ[index] = geometry->Position.Z;
            _radius[index] = geometry->Radius;

            _geometries[index] = geometry;
        }

        BoundingBoxT<real> CalculateBoundingBox() const override
        {
            BoundingBoxT<real> boundingBox = BoundingBoxT<real>::ReverseInfinity();

            for (int i = 0; i < Size; i++)
            {
                if (!_geometries[i])
                {
                    break;
                }

                boundingBox = boundingBox.Union(_geometries[i]->CalculateBoundingBox());
            }

            return boundingBox;
        }

        constexpr IntersectionResult IntersectEntrance(const Ray& ray) const override
        {
            return Intersect<IntersectionResultType::Entrance>(ray);
        }

        constexpr IntersectionResult IntersectExit(const Ray& ray) const override
        {
            return Intersect<IntersectionResultType::Exit>(ray);
        }

    private:
        template <IntersectionResultType TIntersectionResultType>
        force_inline constexpr IntersectionResult Intersect(const Ray& ray) const
        {
            if (std::is_constant_evaluated())
            {
                real closestDistance = std::numeric_limits<real>::infinity();
                const Sphere* closestGeometry = nullptr;

                for (int i = 0; i < Size; i++)
                {
                    auto geometry = _geometries[i];

                    if (geometry == nullptr)
                    {
                        continue;
                    }

                    real distance = geometry->Intersect<TIntersectionResultType>(ray);

                    if (distance < closestDistance)
                    {
                        closestDistance = distance;
                        closestGeometry = geometry;
                    }
                }

                return {closestGeometry, closestDistance};
            }
            else
            {
                VclVec rayPositionX{ray.Position.X};
                VclVec rayPositionY{ray.Position.Y};
                VclVec rayPositionZ{ray.Position.Z};

                VclVec spherePositionX = VclVec{}.load_a(_positionX);
                VclVec spherePositionY = VclVec{}.load_a(_positionY);
                VclVec spherePositionZ = VclVec{}.load_a(_positionZ);

                VclVec vX = rayPositionX - spherePositionX;
                VclVec vY = rayPositionY - spherePositionY;
                VclVec vZ = rayPositionZ - spherePositionZ;

                VclVec rayDirectionX{ray.Direction.X};
                VclVec rayDirectionY{ray.Direction.Y};
                VclVec rayDirectionZ{ray.Direction.Z};

                VclVec sphereRadius = VclVec{}.load_a(_radius);

                VclVec a = SimdDot(rayDirectionX, rayDirectionY, rayDirectionZ, rayDirectionX, rayDirectionY, rayDirectionZ);
                VclVec b = SimdDot(vX, vY, vZ, rayDirectionX, rayDirectionY, rayDirectionZ);
                VclVec c = SimdDot(vX, vY, vZ, vX, vY, vZ) - (sphereRadius * sphereRadius);

                VclVec discriminant = (b * b) - (a * c);
                VclVec discriminantSqrt = sqrt(discriminant);

                VclVec reciprocalA = approx_recipr(a);
                VclVec negativeB = -b;

                VclVec exitDistance = (negativeB + discriminantSqrt) * reciprocalA;

                VclVec result;
                if constexpr (TIntersectionResultType == IntersectionResultType::Entrance)
                {
                    VclVec entranceDistance = (negativeB - discriminantSqrt) * reciprocalA;
                    result = entranceDistance;
                }
                else
                {
                    result = exitDistance;
                }

                // Make sure infinite8f() is second so nans are replaced with inf.
                VclVec clampedResult = select(exitDistance >= VclVec{real{0.0}}, result, VclVec{std::numeric_limits<real>::infinity()});

                real minimumEntranceDistance = horizontal_min1(clampedResult);
                int minimumIndex = horizontal_find_first(VclVec{minimumEntranceDistance} == clampedResult);

                return {
                    _geometries[minimumIndex == -1 ? 0 : minimumIndex],
                    minimumEntranceDistance,
                };
            }
        }
    };
}