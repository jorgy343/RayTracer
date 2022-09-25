module;

#include "Vcl.h"

export module AxisAlignedBoxSoa;

import <cassert>;
import <cmath>;
import <initializer_list>;
import <memory>;

import "Common.h";

import Alignment;
import AxisAlignedBox;
import GeometrySoa;
import IntersectionResult;
import IntersectionResultType;
import Math;
import Ray;

using namespace vcl;

namespace Yart
{
    export class __declspec(dllexport) alignas(64) AxisAlignedBoxSoa final : public GeometrySoa<AxisAlignedBox>
    {
    private:
        alignas(16) float _minimumX[8];
        alignas(16) float _minimumY[8];
        alignas(16) float _minimumZ[8];

        alignas(16) float _maximumX[8];
        alignas(16) float _maximumY[8];
        alignas(16) float _maximumZ[8];

        alignas(16) const AxisAlignedBox* _geometries[8];

    public:
        constexpr AxisAlignedBoxSoa()
        {
            for (int i = 0; i < 8; i++)
            {
                _minimumX[i] = std::numeric_limits<float>::infinity();
                _minimumY[i] = std::numeric_limits<float>::infinity();
                _minimumZ[i] = std::numeric_limits<float>::infinity();

                _maximumX[i] = std::numeric_limits<float>::infinity();
                _maximumY[i] = std::numeric_limits<float>::infinity();
                _maximumZ[i] = std::numeric_limits<float>::infinity();

                _geometries[i] = nullptr;
            }
        }

		constexpr explicit AxisAlignedBoxSoa(std::initializer_list<const AxisAlignedBox*> list)
			: AxisAlignedBoxSoa{}
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

        constexpr void Insert(size_t index, const AxisAlignedBox* geometry) override final
        {
			assert(index >= 0 && index < 8);

            _minimumX[index] = geometry->Minimum.X;
            _minimumY[index] = geometry->Minimum.Y;
            _minimumZ[index] = geometry->Minimum.Z;

            _maximumX[index] = geometry->Maximum.X;
            _maximumY[index] = geometry->Maximum.Y;
            _maximumZ[index] = geometry->Maximum.Z;

            _geometries[index] = geometry;
        }

        IntersectionResult IntersectEntrance(const Ray& ray) const override final
        {
            return Intersect<IntersectionResultType::Entrance>(ray);
        }

        IntersectionResult IntersectExit(const Ray& ray) const override final
        {
            return Intersect<IntersectionResultType::Exit>(ray);
        }

    private:
        template <IntersectionResultType TIntersectionResultType>
        force_inline IntersectionResult Intersect(const Ray& ray) const
        {
            Vec8f minimumX = Vec8f{}.load_a(_minimumX);
            Vec8f minimumY = Vec8f{}.load_a(_minimumY);
            Vec8f minimumZ = Vec8f{}.load_a(_minimumZ);

            Vec8f rayPositionX{ray.Position.X};
            Vec8f rayPositionY{ray.Position.Y};
            Vec8f rayPositionZ{ray.Position.Z};

            Vec8f rayInverseDirectionX{ray.InverseDirection.X};
            Vec8f rayInverseDirectionY{ray.InverseDirection.Y};
            Vec8f rayInverseDirectionZ{ray.InverseDirection.Z};

            Vec8f minX = ConvertNanToInf((minimumX - rayPositionX) * rayInverseDirectionX);
            Vec8f minY = ConvertNanToInf((minimumY - rayPositionY) * rayInverseDirectionY);
            Vec8f minZ = ConvertNanToInf((minimumZ - rayPositionZ) * rayInverseDirectionZ);

            Vec8f maximumX = Vec8f{}.load_a(_maximumX);
            Vec8f maximumY = Vec8f{}.load_a(_maximumY);
            Vec8f maximumZ = Vec8f{}.load_a(_maximumZ);

            Vec8f maxX = ConvertNanToInf((maximumX - rayPositionX) * rayInverseDirectionX);
            Vec8f maxY = ConvertNanToInf((maximumY - rayPositionY) * rayInverseDirectionY);
            Vec8f maxZ = ConvertNanToInf((maximumZ - rayPositionZ) * rayInverseDirectionZ);

            Vec8f exitDistance = vcl::min(vcl::min(vcl::max(minX, maxX), vcl::max(minY, maxY)), vcl::max(minZ, maxZ));
            Vec8f entranceDistance = vcl::max(vcl::max(vcl::min(minX, maxX), vcl::min(minY, maxY)), vcl::min(minZ, maxZ));

            Vec8f result;
            if constexpr (TIntersectionResultType == IntersectionResultType::Entrance)
            {
                result = entranceDistance;
            }
            else
            {
                result = exitDistance;
            }

            Vec8f clampedEntranceDistance = select(exitDistance >= Vec8f(0.0f) & entranceDistance <= exitDistance, result, infinite8f());
            
            float minimumEntranceDistance = horizontal_min1(clampedEntranceDistance);
            int minimumIndex = horizontal_find_first(Vec8f(minimumEntranceDistance) == clampedEntranceDistance);

            return {
                _geometries[minimumIndex == -1 ? 0 : minimumIndex],
                minimumEntranceDistance,
            };
        }
    };
}