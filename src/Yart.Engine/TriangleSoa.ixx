module;

#include "Vcl.h"

export module TriangleSoa;

import <cassert>;
import <initializer_list>;
import <limits>;

import "Common.h";

import BoundingBox;
import GeometrySoa;
import IntersectionResult;
import IntersectionResultType;
import Math;
import Ray;
import Triangle;

using namespace vcl;

namespace Yart
{
    export class __declspec(dllexport) alignas(64) TriangleSoa : public GeometrySoa<Triangle>
    {
    public:
        static constexpr size_t Size = std::same_as<real, float> ? 8 : 4;

    private:
        using VclVec = typename std::conditional<std::same_as<real, float>, Vec8f, Vec4d>::type;

        alignas(Size * 4) real _vertex0X[Size];
        alignas(Size * 4) real _vertex0Y[Size];
        alignas(Size * 4) real _vertex0Z[Size];

        alignas(Size * 4) real _vertex1X[Size];
        alignas(Size * 4) real _vertex1Y[Size];
        alignas(Size * 4) real _vertex1Z[Size];

        alignas(Size * 4) real _vertex2X[Size];
        alignas(Size * 4) real _vertex2Y[Size];
        alignas(Size * 4) real _vertex2Z[Size];

        const Triangle* _geometries[Size];

    public:
        constexpr TriangleSoa()
        {
            for (int i = 0; i < Size; i++)
            {
                _vertex0X[i] = std::numeric_limits<real>::infinity();
                _vertex0Y[i] = std::numeric_limits<real>::infinity();
                _vertex0Z[i] = std::numeric_limits<real>::infinity();

                _vertex1X[i] = std::numeric_limits<real>::infinity();
                _vertex1Y[i] = std::numeric_limits<real>::infinity();
                _vertex1Z[i] = std::numeric_limits<real>::infinity();

                _vertex2X[i] = std::numeric_limits<real>::infinity();
                _vertex2Y[i] = std::numeric_limits<real>::infinity();
                _vertex2Z[i] = std::numeric_limits<real>::infinity();

                _geometries[i] = nullptr;
            }
        }

        constexpr explicit TriangleSoa(std::initializer_list<const Triangle*> list)
            : TriangleSoa{}
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

        constexpr void Insert(size_t index, const Triangle* geometry) override
        {
            assert(index >= 0 && index < Size);

            _vertex0X[index] = geometry->Vertex0.X;
            _vertex0Y[index] = geometry->Vertex0.Y;
            _vertex0Z[index] = geometry->Vertex0.Z;

            _vertex1X[index] = geometry->Vertex1.X;
            _vertex1Y[index] = geometry->Vertex1.Y;
            _vertex1Z[index] = geometry->Vertex1.Z;

            _vertex2X[index] = geometry->Vertex2.X;
            _vertex2Y[index] = geometry->Vertex2.Y;
            _vertex2Z[index] = geometry->Vertex2.Z;

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
            VclVec vertex0X = VclVec{}.load_a(_vertex0X);
            VclVec vertex0Y = VclVec{}.load_a(_vertex0Y);
            VclVec vertex0Z = VclVec{}.load_a(_vertex0Z);

            VclVec vertex1X = VclVec{}.load_a(_vertex1X);
            VclVec vertex1Y = VclVec{}.load_a(_vertex1Y);
            VclVec vertex1Z = VclVec{}.load_a(_vertex1Z);

            VclVec edge1X = vertex1X - vertex0X;
            VclVec edge1Y = vertex1Y - vertex0Y;
            VclVec edge1Z = vertex1Z - vertex0Z;

            VclVec vertex2X = VclVec{}.load_a(_vertex2X);
            VclVec vertex2Y = VclVec{}.load_a(_vertex2Y);
            VclVec vertex2Z = VclVec{}.load_a(_vertex2Z);

            VclVec edge2X = vertex2X - vertex0X;
            VclVec edge2Y = vertex2Y - vertex0Y;
            VclVec edge2Z = vertex2Z - vertex0Z;

            VclVec rayDirectionX{ray.Direction.X};
            VclVec rayDirectionY{ray.Direction.Y};
            VclVec rayDirectionZ{ray.Direction.Z};

            auto h = SimdCrossProduct(rayDirectionX, rayDirectionY, rayDirectionZ, edge2X, edge2Y, edge2Z);
            VclVec a = SimdDot(edge1X, edge1Y, edge1Z, h.X, h.Y, h.Z);

            // Normally you would check for a parallel ray here but we'll skip that check.

            VclVec f = approx_recipr(a);

            VclVec rayPositionX{ray.Position.X};
            VclVec rayPositionY{ray.Position.Y};
            VclVec rayPositionZ{ray.Position.Z};

            VclVec sX = rayPositionX - vertex0X;
            VclVec sY = rayPositionY - vertex0Y;
            VclVec sZ = rayPositionZ - vertex0Z;

            VclVec u = f * SimdDot(sX, sY, sZ, h.X, h.Y, h.Z);
            auto q = SimdCrossProduct(sX, sY, sZ, edge1X, edge1Y, edge1Z);
            VclVec v = f * SimdDot(rayDirectionX, rayDirectionY, rayDirectionZ, q.X, q.Y, q.Z);

            VclVec entranceDistance = f * SimdDot(edge2X, edge2Y, edge2Z, q.X, q.Y, q.Z);

            // Make sure infinity is second so nans are replaced with inf.
            VclVec clampedEntranceDistance = select(
                u >= VclVec{0.0f} && u <= VclVec{1.0f} && v >= VclVec{0.0f} && u + v <= VclVec{1.0f} && entranceDistance >= VclVec{0.0f},
                entranceDistance,
                VclVec{std::numeric_limits<real>::infinity()});

            real minimumEntranceDistance = horizontal_min1(clampedEntranceDistance);
            int minimumIndex = horizontal_find_first(VclVec{minimumEntranceDistance} == clampedEntranceDistance);

            return {
                _geometries[minimumIndex == -1 ? 0 : minimumIndex],
                minimumEntranceDistance,
            };
        }
    };
}