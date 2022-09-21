module;

#include "Vcl.h"

export module SphereSoa;

import <cassert>;
import <cmath>;
import <initializer_list>;

import "Common.h";

import Alignment;
import GeometrySoa;
import IntersectionResult;
import IntersectionResultType;
import Math;
import Ray;
import Sphere;

using namespace vcl;

namespace Yart
{
    export class __declspec(dllexport) alignas(64) SphereSoa final : public GeometrySoa<Sphere>
    {
    private:
        alignas(16) float _positionX[8];
        alignas(16) float _positionY[8];
        alignas(16) float _positionZ[8];
        alignas(16) float _radius[8];
        alignas(16) const Sphere* _geometries[8];

    public:
        constexpr SphereSoa()
        {
            for (int i = 0; i < 8; i++)
            {
                _positionX[i] = std::numeric_limits<float>::infinity();
                _positionY[i] = std::numeric_limits<float>::infinity();
                _positionZ[i] = std::numeric_limits<float>::infinity();
                _radius[i] = std::numeric_limits<float>::infinity();
                _geometries[i] = nullptr;
            }
        }

        constexpr explicit SphereSoa(std::initializer_list<const Sphere*> list)
			: SphereSoa{}
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

        constexpr void Insert(int index, const Sphere* geometry) override final
        {
            assert(index >= 0 && index < 8);

            _positionX[index] = geometry->Position.X;
            _positionY[index] = geometry->Position.Y;
            _positionZ[index] = geometry->Position.Z;
            _radius[index] = geometry->Radius;
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
            if (std::is_constant_evaluated())
            {
                float closestDistance = std::numeric_limits<float>::infinity();
                const Sphere* closestGeometry = nullptr;

                for (int i = 0; i < 8; i++)
                {
                    auto geometry = _geometries[i];

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

                return {closestGeometry, closestDistance};
            }
            else
            {
                Vec8f rayPositionX{ray.Position.X};
                Vec8f rayPositionY{ray.Position.Y};
                Vec8f rayPositionZ{ray.Position.Z};

                Vec8f spherePositionX = Vec8f{}.load_a(_positionX);
                Vec8f spherePositionY = Vec8f{}.load_a(_positionY);
                Vec8f spherePositionZ = Vec8f{}.load_a(_positionZ);

                Vec8f vX = rayPositionX - spherePositionX;
                Vec8f vY = rayPositionY - spherePositionY;
                Vec8f vZ = rayPositionZ - spherePositionZ;

                Vec8f rayDirectionX{ray.Direction.X};
                Vec8f rayDirectionY{ray.Direction.Y};
                Vec8f rayDirectionZ{ray.Direction.Z};

                Vec8f sphereRadius = Vec8f{}.load_a(_radius);

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
                    _geometries[minimumIndex == -1 ? 0 : minimumIndex],
                    minimumEntranceDistance,
                };
            }
        }
    };
}