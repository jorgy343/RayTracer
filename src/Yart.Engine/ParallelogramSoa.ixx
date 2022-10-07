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

using namespace vcl;

namespace Yart
{
    export class alignas(64) ParallelogramSoa : public GeometrySoa<Parallelogram>
    {
    public:
        static constexpr size_t Size = std::same_as<real, float> ? 8 : 4;

    private:
        using VclVec = typename std::conditional<std::same_as<real, float>, Vec8f, Vec4d>::type;

        alignas(sizeof(real) * 4) real _positionX[Size];
        alignas(sizeof(real) * 4) real _positionY[Size];
        alignas(sizeof(real) * 4) real _positionZ[Size];

        alignas(sizeof(real) * 4) real _edge1X[Size];
        alignas(sizeof(real) * 4) real _edge1Y[Size];
        alignas(sizeof(real) * 4) real _edge1Z[Size];

        alignas(sizeof(real) * 4) real _edge2X[Size];
        alignas(sizeof(real) * 4) real _edge2Y[Size];
        alignas(sizeof(real) * 4) real _edge2Z[Size];

        alignas(16) const Parallelogram* _geometries[Size];

    public:
        constexpr ParallelogramSoa()
        {
            for (int i = 0; i < Size; i++)
            {
                _positionX[i] = std::numeric_limits<real>::infinity();
                _positionY[i] = std::numeric_limits<real>::infinity();
                _positionZ[i] = std::numeric_limits<real>::infinity();

                _edge1X[i] = std::numeric_limits<real>::infinity();
                _edge1Y[i] = std::numeric_limits<real>::infinity();
                _edge1Z[i] = std::numeric_limits<real>::infinity();

                _edge2X[i] = std::numeric_limits<real>::infinity();
                _edge2Y[i] = std::numeric_limits<real>::infinity();
                _edge2Z[i] = std::numeric_limits<real>::infinity();

                _geometries[i] = nullptr;
            }
        }

        constexpr explicit ParallelogramSoa(std::initializer_list<const Parallelogram*> list)
            : ParallelogramSoa{}
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

        constexpr void Insert(size_t index, const Parallelogram* geometry) override
        {
            assert(index >= 0 && index < Size);

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
                real closestDistance = std::numeric_limits<real>::infinity();
                const Parallelogram* closestGeometry = nullptr;

                for (int i = 0; i < Size; i++)
                {
                    auto geometry = _geometries[i];

                    if (geometry == nullptr)
                    {
                        continue;
                    }

                    real distance = geometry->Intersect(ray);

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
                VclVec rayDirectionX{ray.Direction.X};
                VclVec rayDirectionY{ray.Direction.Y};
                VclVec rayDirectionZ{ray.Direction.Z};

                VclVec edge2X = VclVec{}.load_a(_edge2X);
                VclVec edge2Y = VclVec{}.load_a(_edge2Y);
                VclVec edge2Z = VclVec{}.load_a(_edge2Z);

                auto p = SimdCrossProduct(rayDirectionX, rayDirectionY, rayDirectionZ, edge2X, edge2Y, edge2Z);

                VclVec edge1X = VclVec{}.load_a(_edge1X);
                VclVec edge1Y = VclVec{}.load_a(_edge1Y);
                VclVec edge1Z = VclVec{}.load_a(_edge1Z);

                VclVec determinant = SimdDot(edge1X, edge1Y, edge1Z, p.X, p.Y, p.Z);
                VclVec invDeterminant = approx_recipr(determinant);

                VclVec rayPositionX{ray.Position.X};
                VclVec rayPositionY{ray.Position.Y};
                VclVec rayPositionZ{ray.Position.Z};

                VclVec positionX = VclVec{}.load_a(_positionX);
                VclVec positionY = VclVec{}.load_a(_positionY);
                VclVec positionZ = VclVec{}.load_a(_positionZ);

                VclVec tX = rayPositionX - positionX;
                VclVec tY = rayPositionY - positionY;
                VclVec tZ = rayPositionZ - positionZ;

                VclVec a = SimdDot(tX, tY, tZ, p.X, p.Y, p.Z) * invDeterminant;
                auto q = SimdCrossProduct(tX, tY, tZ, edge1X, edge1Y, edge1Z);
                VclVec b = SimdDot(rayDirectionX, rayDirectionY, rayDirectionZ, q.X, q.Y, q.Z) * invDeterminant;

                VclVec entranceDistance = SimdDot(edge2X, edge2Y, edge2Z, q.X, q.Y, q.Z) * invDeterminant;

                auto comparison =
                    entranceDistance >= VclVec(real{0.0}) &&
                    a >= VclVec(real{0.0}) &&
                    a <= VclVec(real{1.0}) &&
                    b >= VclVec(real{0.0}) &&
                    b <= VclVec(real{1.0});

                // Make sure infinite8f() is second so nans are replaced with inf.
                VclVec clampedEntranceDistance = select(comparison, entranceDistance, VclVec{std::numeric_limits<real>::infinity()});

                real minimumEntranceDistance = horizontal_min1(clampedEntranceDistance);
                int minimumIndex = horizontal_find_first(VclVec{minimumEntranceDistance} == clampedEntranceDistance);

                return {
                    _geometries[minimumIndex == -1 ? 0 : minimumIndex],
                    minimumEntranceDistance,
                };
            }
        }
    };
}