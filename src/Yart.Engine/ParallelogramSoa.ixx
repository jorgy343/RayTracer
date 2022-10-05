module;

#include "Vcl.h"

export module ParallelogramSoa;

import <cassert>;
import <initializer_list>;

import "Common.h";

import Alignment;
import BoundingBox;
import GeometrySoa;
import IntersectionResult;
import IntersectionResultType;
import Math;
import Parallelogram;
import Ray;
import Vec8f3;

using namespace vcl;

namespace Yart
{
    export class alignas(64) ParallelogramSoa : public GeometrySoa<Parallelogram>
    {
    private:
        alignas(16) float _positionX[8];
        alignas(16) float _positionY[8];
        alignas(16) float _positionZ[8];

        alignas(16) float _edge1X[8];
        alignas(16) float _edge1Y[8];
        alignas(16) float _edge1Z[8];

        alignas(16) float _edge2X[8];
        alignas(16) float _edge2Y[8];
        alignas(16) float _edge2Z[8];

        alignas(16) const Parallelogram* _geometries[8];

    public:
        constexpr ParallelogramSoa()
        {
            for (int i = 0; i < 8; i++)
            {
                _positionX[i] = std::numeric_limits<float>::infinity();
                _positionY[i] = std::numeric_limits<float>::infinity();
                _positionZ[i] = std::numeric_limits<float>::infinity();

                _edge1X[i] = std::numeric_limits<float>::infinity();
                _edge1Y[i] = std::numeric_limits<float>::infinity();
                _edge1Z[i] = std::numeric_limits<float>::infinity();

                _edge2X[i] = std::numeric_limits<float>::infinity();
                _edge2Y[i] = std::numeric_limits<float>::infinity();
                _edge2Z[i] = std::numeric_limits<float>::infinity();

                _geometries[i] = nullptr;
            }
        }

        constexpr explicit ParallelogramSoa(std::initializer_list<const Parallelogram*> list)
			: ParallelogramSoa{}
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

        constexpr void Insert(size_t index, const Parallelogram* geometry) override
        {
            assert(index >= 0 && index < 8);

            _positionX[index] = geometry->Position.X;
            _positionY[index] = geometry->Position.Y;
            _positionZ[index] = geometry->Position.Z;

            _edge1X[index] = geometry->Edge1.X;
            _edge1Y[index] = geometry->Edge1.Y;
            _edge1Z[index] = geometry->Edge1.Z;

            _edge2X[index] = geometry->Edge2.X;
            _edge2Y[index] = geometry->Edge2.Y;
            _edge2Z[index] = geometry->Edge2.Z;

            _geometries[index] = geometry;
        }

        BoundingBox CalculateBoundingBox() const override
        {
            BoundingBox boundingBox = BoundingBox::ReverseInfinity();

            for (int i = 0; i < 8; i++)
            {
                if (!_geometries[i])
                {
                    break;
                }

                boundingBox = boundingBox.Union(_geometries[i]->CalculateBoundingBox());
            }

            return boundingBox;
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
            if (std::is_constant_evaluated())
            {
                float closestDistance = std::numeric_limits<float>::infinity();
                const Parallelogram* closestGeometry = nullptr;

                for (int i = 0; i < 8; i++)
                {
                    auto geometry = _geometries[i];

                    if (geometry == nullptr)
                    {
                        continue;
                    }

                    float distance = geometry->Intersect(ray);

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
                Vec8f rayDirectionX{ray.Direction.X};
                Vec8f rayDirectionY{ray.Direction.Y};
                Vec8f rayDirectionZ{ray.Direction.Z};

                Vec8f edge2X = Vec8f{}.load_a(_edge2X);
                Vec8f edge2Y = Vec8f{}.load_a(_edge2Y);
                Vec8f edge2Z = Vec8f{}.load_a(_edge2Z);

                Vec8f3 p = SimdCrossProduct(rayDirectionX, rayDirectionY, rayDirectionZ, edge2X, edge2Y, edge2Z);

                Vec8f edge1X = Vec8f{}.load_a(_edge1X);
                Vec8f edge1Y = Vec8f{}.load_a(_edge1Y);
                Vec8f edge1Z = Vec8f{}.load_a(_edge1Z);

                Vec8f determinant = SimdDot(edge1X, edge1Y, edge1Z, p.X, p.Y, p.Z);
                Vec8f invDeterminant = approx_recipr(determinant);

                Vec8f rayPositionX{ray.Position.X};
                Vec8f rayPositionY{ray.Position.Y};
                Vec8f rayPositionZ{ray.Position.Z};

                Vec8f positionX = Vec8f{}.load_a(_positionX);
                Vec8f positionY = Vec8f{}.load_a(_positionY);
                Vec8f positionZ = Vec8f{}.load_a(_positionZ);

                Vec8f tX = rayPositionX - positionX;
                Vec8f tY = rayPositionY - positionY;
                Vec8f tZ = rayPositionZ - positionZ;

                Vec8f a = SimdDot(tX, tY, tZ, p.X, p.Y, p.Z) * invDeterminant;
                Vec8f3 q = SimdCrossProduct(tX, tY, tZ, edge1X, edge1Y, edge1Z);
                Vec8f b = SimdDot(rayDirectionX, rayDirectionY, rayDirectionZ, q.X, q.Y, q.Z) * invDeterminant;

                Vec8f entranceDistance = SimdDot(edge2X, edge2Y, edge2Z, q.X, q.Y, q.Z) * invDeterminant;

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
                    _geometries[minimumIndex == -1 ? 0 : minimumIndex],
                    minimumEntranceDistance,
                };
            }
        }
    };
}