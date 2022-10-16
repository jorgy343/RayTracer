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
    export template<SoaSize Size>
        class __declspec(dllexport) alignas(64) TriangleSoa : public GeometrySoa<Triangle>
    {
    public:
        static constexpr size_t Elements = std::same_as<real, float> ? (Size == SoaSize::_256 ? 8 : 4) : (Size == SoaSize::_256 ? 4 : 2);

    private:
        using VclVec = std::conditional_t<std::same_as<real, float>, std::conditional_t<Size == SoaSize::_256, Vec8f, Vec4f>, std::conditional_t<Size == SoaSize::_256, Vec4d, Vec2d>>;

        alignas(Elements * sizeof(real)) real _vertex0X[Elements];
        alignas(Elements * sizeof(real)) real _vertex0Y[Elements];
        alignas(Elements * sizeof(real)) real _vertex0Z[Elements];

        alignas(Elements * sizeof(real)) real _vertex1X[Elements];
        alignas(Elements * sizeof(real)) real _vertex1Y[Elements];
        alignas(Elements * sizeof(real)) real _vertex1Z[Elements];

        alignas(Elements * sizeof(real)) real _vertex2X[Elements];
        alignas(Elements * sizeof(real)) real _vertex2Y[Elements];
        alignas(Elements * sizeof(real)) real _vertex2Z[Elements];

        const Triangle* _geometries[Elements];

    public:
        constexpr TriangleSoa()
        {
            for (int i = 0; i < Elements; i++)
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
                if (index >= Elements)
                {
                    break;
                }

                Insert(index++, geometry);
            }
        }

        constexpr void Insert(size_t index, const Triangle* geometry) override
        {
            assert(index >= 0 && index < Elements);

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
            VectorVec3<VclVec> vertex0{_vertex0X, _vertex0Y, _vertex0Z};
            VectorVec3<VclVec> vertex1{_vertex1X, _vertex1Y, _vertex1Z};
            VectorVec3<VclVec> vertex2{_vertex2X, _vertex2Y, _vertex2Z};

            VectorVec3<VclVec> edge1 = vertex1 - vertex0;
            VectorVec3<VclVec> edge2 = vertex2 - vertex0;

            VectorVec3<VclVec> rayDirection{ray.Direction};
            VectorVec3<VclVec> rayPosition{ray.Position};

            VectorVec3<VclVec> h = rayDirection % edge2;
            VclVec a = VectorVec3<VclVec>::Dot(edge1, h);

            // Normally you would check for a parallel ray here but we'll skip that check.

            VclVec f = approx_recipr(a);

            VectorVec3<VclVec> s = rayPosition - vertex0;

            VclVec u = f * VectorVec3<VclVec>::Dot(s, h);
            VectorVec3<VclVec> q = s % edge1;
            VclVec v = f * VectorVec3<VclVec>::Dot(rayDirection, q);

            VclVec entranceDistance = f * VectorVec3<VclVec>::Dot(edge2, q);

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