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
import Vec8f3;

using namespace vcl;

namespace Yart
{
    export class __declspec(dllexport) alignas(64) TriangleSoa : public GeometrySoa<Triangle>
    {
    private:
        alignas(16) float _vertex0X[8];
        alignas(16) float _vertex0Y[8];
        alignas(16) float _vertex0Z[8];

        alignas(16) float _vertex1X[8];
        alignas(16) float _vertex1Y[8];
        alignas(16) float _vertex1Z[8];

        alignas(16) float _vertex2X[8];
        alignas(16) float _vertex2Y[8];
        alignas(16) float _vertex2Z[8];

        alignas(16) const Triangle* _geometries[8];

    public:
        constexpr TriangleSoa()
        {
            for (int i = 0; i < 8; i++)
            {
                _vertex0X[i] = std::numeric_limits<float>::infinity();
                _vertex0Y[i] = std::numeric_limits<float>::infinity();
                _vertex0Z[i] = std::numeric_limits<float>::infinity();

                _vertex1X[i] = std::numeric_limits<float>::infinity();
                _vertex1Y[i] = std::numeric_limits<float>::infinity();
                _vertex1Z[i] = std::numeric_limits<float>::infinity();

                _vertex2X[i] = std::numeric_limits<float>::infinity();
                _vertex2Y[i] = std::numeric_limits<float>::infinity();
                _vertex2Z[i] = std::numeric_limits<float>::infinity();

                _geometries[i] = nullptr;
            }
        }

        constexpr explicit TriangleSoa(std::initializer_list<const Triangle*> list)
            : TriangleSoa{}
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

        constexpr void Insert(size_t index, const Triangle* geometry) override
        {
            assert(index >= 0 && index < 8);

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
            Vec8f vertex0X = Vec8f{}.load_a(_vertex0X);
            Vec8f vertex0Y = Vec8f{}.load_a(_vertex0Y);
            Vec8f vertex0Z = Vec8f{}.load_a(_vertex0Z);

            Vec8f vertex1X = Vec8f{}.load_a(_vertex1X);
            Vec8f vertex1Y = Vec8f{}.load_a(_vertex1Y);
            Vec8f vertex1Z = Vec8f{}.load_a(_vertex1Z);

            Vec8f edge1X = vertex1X - vertex0X;
            Vec8f edge1Y = vertex1Y - vertex0Y;
            Vec8f edge1Z = vertex1Z - vertex0Z;

            Vec8f vertex2X = Vec8f{}.load_a(_vertex2X);
            Vec8f vertex2Y = Vec8f{}.load_a(_vertex2Y);
            Vec8f vertex2Z = Vec8f{}.load_a(_vertex2Z);

            Vec8f edge2X = vertex2X - vertex0X;
            Vec8f edge2Y = vertex2Y - vertex0Y;
            Vec8f edge2Z = vertex2Z - vertex0Z;

            Vec8f rayDirectionX{ray.Direction.X};
            Vec8f rayDirectionY{ray.Direction.Y};
            Vec8f rayDirectionZ{ray.Direction.Z};

            Vec8f3 h = SimdCrossProduct(rayDirectionX, rayDirectionY, rayDirectionZ, edge2X, edge2Y, edge2Z);
            Vec8f a = SimdDot(edge1X, edge1Y, edge1Z, h.X, h.Y, h.Z);

            // Normally you would check for a parallel ray here but we'll skip that check.

            Vec8f f = approx_recipr(a);

            Vec8f rayPositionX{ray.Position.X};
            Vec8f rayPositionY{ray.Position.Y};
            Vec8f rayPositionZ{ray.Position.Z};

            Vec8f sX = rayPositionX - vertex0X;
            Vec8f sY = rayPositionY - vertex0Y;
            Vec8f sZ = rayPositionZ - vertex0Z;

            Vec8f u = f * SimdDot(sX, sY, sZ, h.X, h.Y, h.Z);
            Vec8f3 q = SimdCrossProduct(sX, sY, sZ, edge1X, edge1Y, edge1Z);
            Vec8f v = f * SimdDot(rayDirectionX, rayDirectionY, rayDirectionZ, q.X, q.Y, q.Z);

            Vec8f entranceDistance = f * SimdDot(edge2X, edge2Y, edge2Z, q.X, q.Y, q.Z);

            // Make sure infinite8f() is second so nans are replaced with inf.
            Vec8f clampedEntranceDistance = select(
                u >= Vec8f(0.0f) && u <= Vec8f(1.0f) && v >= Vec8f(0.0f) && u + v <= Vec8f(1.0f) && entranceDistance >= Vec8f(0.0f),
                entranceDistance,
                infinite8f());

            float minimumEntranceDistance = horizontal_min1(clampedEntranceDistance);
            int minimumIndex = horizontal_find_first(Vec8f(minimumEntranceDistance) == clampedEntranceDistance);

            return {
                _geometries[minimumIndex == -1 ? 0 : minimumIndex],
                minimumEntranceDistance,
            };
        }
    };
}