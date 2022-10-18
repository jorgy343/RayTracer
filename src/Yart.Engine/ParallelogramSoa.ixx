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
    export template<SoaSize Size>
        class alignas(64) ParallelogramSoa : public GeometrySoa<Parallelogram>
    {
    public:
        static constexpr size_t Elements = std::same_as<real, float> ? (Size == SoaSize::_256 ? 8 : 4) : (Size == SoaSize::_256 ? 4 : 2);

    private:
        using VclVec = std::conditional_t<std::same_as<real, float>, std::conditional_t<Size == SoaSize::_256, Vec8f, Vec4f>, std::conditional_t<Size == SoaSize::_256, Vec4d, Vec2d>>;

        alignas(Elements * sizeof(real)) real _positionX[Elements];
        alignas(Elements * sizeof(real)) real _positionY[Elements];
        alignas(Elements * sizeof(real)) real _positionZ[Elements];

        alignas(Elements * sizeof(real)) real _edge1X[Elements];
        alignas(Elements * sizeof(real)) real _edge1Y[Elements];
        alignas(Elements * sizeof(real)) real _edge1Z[Elements];

        alignas(Elements * sizeof(real)) real _edge2X[Elements];
        alignas(Elements * sizeof(real)) real _edge2Y[Elements];
        alignas(Elements * sizeof(real)) real _edge2Z[Elements];

        const Parallelogram* _geometries[Elements];

    public:
        constexpr ParallelogramSoa()
        {
            for (int i = 0; i < Elements; i++)
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
                if (index >= Elements)
                {
                    break;
                }

                Insert(index++, geometry);
            }
        }

        constexpr void Insert(size_t index, const Parallelogram* geometry) override
        {
            assert(index >= 0 && index < Elements);

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

            for (int i = 0; i < Elements; i++)
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
            VectorVec3<VclVec> rayDirection{ray.Direction};
            VectorVec3<VclVec> rayPosition{ray.Position};

            VectorVec3<VclVec> position{_positionX, _positionY, _positionZ};
            VectorVec3<VclVec> edge1{_edge1X, _edge1Y, _edge1Z};
            VectorVec3<VclVec> edge2{_edge2X, _edge2Y, _edge2Z};

            VectorVec3<VclVec> p = rayDirection % edge2;

            VclVec determinant = VectorVec3<VclVec>::Dot(edge1, p);
            VclVec invDeterminant = approx_recipr(determinant);

            VectorVec3<VclVec> t = rayPosition - position;

            VclVec a = VectorVec3<VclVec>::Dot(t, p) * invDeterminant;
            VectorVec3<VclVec> q = t % edge1;
            VclVec b = VectorVec3<VclVec>::Dot(rayDirection, q) * invDeterminant;

            VclVec entranceDistance = VectorVec3<VclVec>::Dot(edge2, q) * invDeterminant;

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
    };
}