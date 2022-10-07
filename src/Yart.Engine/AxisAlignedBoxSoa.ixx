module;

#include "Vcl.h"

export module AxisAlignedBoxSoa;

import <cassert>;
import <initializer_list>;

import "Common.h";

import Alignment;
import AxisAlignedBox;
import BoundingBox;
import GeometrySoa;
import IntersectionResult;
import IntersectionResultType;
import Math;
import Ray;

using namespace vcl;

namespace Yart
{
    export class __declspec(dllexport) alignas(64) AxisAlignedBoxSoa : public GeometrySoa<AxisAlignedBox>
    {
    public:
        static constexpr size_t Size = std::same_as<real, float> ? 8 : 4;

    private:
        using VclVec = typename std::conditional<std::same_as<real, float>, Vec8f, Vec4d>::type;

        alignas(sizeof(real) * 4) real _minimumX[Size];
        alignas(sizeof(real) * 4) real _minimumY[Size];
        alignas(sizeof(real) * 4) real _minimumZ[Size];

        alignas(sizeof(real) * 4) real _maximumX[Size];
        alignas(sizeof(real) * 4) real _maximumY[Size];
        alignas(sizeof(real) * 4) real _maximumZ[Size];

        alignas(16) const AxisAlignedBox* _geometries[Size];

    public:
        constexpr AxisAlignedBoxSoa()
        {
            for (int i = 0; i < Size; i++)
            {
                _minimumX[i] = std::numeric_limits<real>::infinity();
                _minimumY[i] = std::numeric_limits<real>::infinity();
                _minimumZ[i] = std::numeric_limits<real>::infinity();

                _maximumX[i] = std::numeric_limits<real>::infinity();
                _maximumY[i] = std::numeric_limits<real>::infinity();
                _maximumZ[i] = std::numeric_limits<real>::infinity();

                _geometries[i] = nullptr;
            }
        }

        constexpr explicit AxisAlignedBoxSoa(std::initializer_list<const AxisAlignedBox*> list)
            : AxisAlignedBoxSoa{}
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

        constexpr void Insert(size_t index, const AxisAlignedBox* geometry) override
        {
            assert(index >= 0 && index < Size);

            _minimumX[index] = geometry->Minimum.X;
            _minimumY[index] = geometry->Minimum.Y;
            _minimumZ[index] = geometry->Minimum.Z;

            _maximumX[index] = geometry->Maximum.X;
            _maximumY[index] = geometry->Maximum.Y;
            _maximumZ[index] = geometry->Maximum.Z;

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

        IntersectionResult IntersectEntrance(const Ray& ray) const override
        {
            return Intersect<IntersectionResultType::Entrance>(ray);
        }

        IntersectionResult IntersectExit(const Ray& ray) const override
        {
            return Intersect<IntersectionResultType::Exit>(ray);
        }

    private:
        template <IntersectionResultType TIntersectionResultType>
        force_inline IntersectionResult Intersect(const Ray& ray) const
        {
            VclVec minimumX = VclVec{}.load_a(_minimumX);
            VclVec minimumY = VclVec{}.load_a(_minimumY);
            VclVec minimumZ = VclVec{}.load_a(_minimumZ);

            VclVec rayPositionX{ray.Position.X};
            VclVec rayPositionY{ray.Position.Y};
            VclVec rayPositionZ{ray.Position.Z};

            VclVec rayInverseDirectionX{ray.InverseDirection.X};
            VclVec rayInverseDirectionY{ray.InverseDirection.Y};
            VclVec rayInverseDirectionZ{ray.InverseDirection.Z};

            VclVec minX = ConvertNanToInf((minimumX - rayPositionX) * rayInverseDirectionX);
            VclVec minY = ConvertNanToInf((minimumY - rayPositionY) * rayInverseDirectionY);
            VclVec minZ = ConvertNanToInf((minimumZ - rayPositionZ) * rayInverseDirectionZ);

            VclVec maximumX = VclVec{}.load_a(_maximumX);
            VclVec maximumY = VclVec{}.load_a(_maximumY);
            VclVec maximumZ = VclVec{}.load_a(_maximumZ);

            VclVec maxX = ConvertNanToInf((maximumX - rayPositionX) * rayInverseDirectionX);
            VclVec maxY = ConvertNanToInf((maximumY - rayPositionY) * rayInverseDirectionY);
            VclVec maxZ = ConvertNanToInf((maximumZ - rayPositionZ) * rayInverseDirectionZ);

            VclVec exitDistance = vcl::min(vcl::min(vcl::max(minX, maxX), vcl::max(minY, maxY)), vcl::max(minZ, maxZ));
            VclVec entranceDistance = vcl::max(vcl::max(vcl::min(minX, maxX), vcl::min(minY, maxY)), vcl::min(minZ, maxZ));

            VclVec result;
            if constexpr (TIntersectionResultType == IntersectionResultType::Entrance)
            {
                result = entranceDistance;
            }
            else
            {
                result = exitDistance;
            }

            VclVec clampedEntranceDistance = select(exitDistance >= VclVec{real{0.0}} &entranceDistance <= exitDistance, result, VclVec{std::numeric_limits<real>::infinity()});

            real minimumEntranceDistance = horizontal_min1(clampedEntranceDistance);
            int minimumIndex = horizontal_find_first(VclVec{minimumEntranceDistance} == clampedEntranceDistance);

            return {
                _geometries[minimumIndex == -1 ? 0 : minimumIndex],
                minimumEntranceDistance,
            };
        }
    };
}