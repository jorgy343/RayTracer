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
    export template<SoaSize Size>
        class __declspec(dllexport) alignas(64) SphereSoa : public GeometrySoa<Sphere>
    {
    public:
        static constexpr size_t Elements = std::same_as<real, float> ? (Size == SoaSize::_256 ? 8 : 4) : (Size == SoaSize::_256 ? 4 : 2);

    private:
        using VclVec = std::conditional_t<std::same_as<real, float>, std::conditional_t<Size == SoaSize::_256, Vec8f, Vec4f>, std::conditional_t<Size == SoaSize::_256, Vec4d, Vec2d>>;

        alignas(Elements * sizeof(real)) real _positionX[Elements];
        alignas(Elements * sizeof(real)) real _positionY[Elements];
        alignas(Elements * sizeof(real)) real _positionZ[Elements];
        alignas(Elements * sizeof(real)) real _radius[Elements];

        const Sphere* _geometries[Elements];

    public:
        constexpr SphereSoa()
        {
            for (int i = 0; i < Elements; i++)
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
                if (index >= Elements)
                {
                    break;
                }

                Insert(index++, geometry);
            }
        }

        constexpr void Insert(size_t index, const Sphere* geometry) override
        {
            assert(index >= 0 && index < Elements);

            _positionX[index] = geometry->Position.X;
            _positionY[index] = geometry->Position.Y;
            _positionZ[index] = geometry->Position.Z;
            _radius[index] = geometry->Radius;

            _geometries[index] = geometry;
        }

        BoundingBoxT<real> CalculateBoundingBox() const override
        {
            BoundingBoxT<real> boundingBox = BoundingBoxT<real>::ReverseInfinity();

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
            VectorVec3<VclVec> rayPosition{ray.Position};
            VectorVec3<VclVec> rayDirection{ray.Direction};

            VectorVec3<VclVec> position{_positionX, _positionY, _positionZ};
            VclVec radius = VclVec{}.load_a(_radius);

            VectorVec3<VclVec> v = rayPosition - position;

            VclVec a = VectorVec3<VclVec>::Dot(rayDirection, rayDirection);
            VclVec b = VectorVec3<VclVec>::Dot(v, rayDirection);
            VclVec c = VectorVec3<VclVec>::Dot(v, v) - (radius * radius);

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
    };
}